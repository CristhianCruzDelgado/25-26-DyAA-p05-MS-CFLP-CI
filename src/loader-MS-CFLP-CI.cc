#include "../include/loader-MS-CFLP-CI.h"
#include "../include/instance-MS-CFLP-CI.h"

// Skip whitespaces from start and end of string
static inline std::string trim(const std::string& str) {
  size_t first = str.find_first_not_of(" \t\r\n");
  if (first == std::string::npos) return "";
  size_t last = str.find_last_not_of(" \t\r\n");
  return str.substr(first, last - first + 1);
}

Instance* LoaderMSCFLPCI::load(const char* path) const {
  std::ifstream file(path);
  if (!file) 
    throw std::runtime_error("Bad path " + std::string(path));
  
  short warehouses = -1, stores = -1;
  std::vector<short> capacity, fixed_cost, goods;
  std::vector<std::vector<short>> supply_cost;
  short incompatibilities = -1;
  std::vector<ShortPair> incompatible_pairs;
  
  std::string buffer;
  while (getline(file, buffer)) {
    std::string line = trim(buffer);
    if (line.empty()) continue;
    if (line.find("SupplyCost") != std::string::npos) {
      size_t eqPos = line.find('=');
      std::string value = trim(line.substr(eqPos + 1));
      supply_cost = loadMatrix(file, value, stores, warehouses);
      continue;
    }
    auto [field, value] = readUntilSemicolon(file, line);
    if      (field == "Warehouses")        warehouses         = readShort(value);
    else if (field == "Stores")            stores             = readShort(value);
    else if (field == "Capacity")          capacity           = readVectorShort(value, warehouses);
    else if (field == "FixedCost")         fixed_cost         = readVectorShort(value, warehouses);
    else if (field == "Goods")             goods              = readVectorShort(value, stores);
    // else if (field == "SupplyCost")     supply_cost        = ^^^ handled separately
    else if (field == "Incompatibilities") incompatibilities  = readInt(value);
    else if (field == "IncompatiblePairs") incompatible_pairs = readVectorPairsShort(value, incompatibilities);
  }
  
  Instance* instance = new InstanceMSCFLPCI(capacity, fixed_cost, goods, supply_cost, incompatible_pairs);
  return instance;
}

StringPair LoaderMSCFLPCI::readUntilSemicolon(std::ifstream& file, const std::string& firstLine) const {
  std::string assignment = firstLine;
  while (assignment.find(';') == std::string::npos) {
    std::string buffer;
    if (!getline(file, buffer)) 
      throw std::runtime_error("Unexpected EOF while reading multi-line value");
    assignment += ' ' + buffer;
  }
  size_t equalPos = assignment.find('=');
  std::string field = assignment.substr(0, equalPos);
  std::string value = assignment.substr(equalPos + 1, assignment.find(';') - equalPos - 1);
  return { trim(field), trim(value) };
}

short LoaderMSCFLPCI::readShort(const std::string& value) const {
  int number = std::stoi(value);
  if (number <= 0) 
    throw std::runtime_error("Invalid value (must be > 0): " + value);
  if (number > std::numeric_limits<short>::max()) 
    throw std::runtime_error("Value exceeds short range: " + value);
  return static_cast<short>(number);
}

std::vector<short> LoaderMSCFLPCI::readVectorShort(const std::string& value, short size) const {
  if (size <= 0) 
    throw std::runtime_error("Invalid size for vector: " + std::to_string(size));
  std::string cleaned = trim(value);
  if (cleaned.front() == '[') cleaned = cleaned.substr(1);
  if (cleaned.back() == ']') cleaned.pop_back();
  if (cleaned.back() == ';') cleaned.pop_back();  
  std::vector<short> vector;
  vector.reserve(size);
  std::stringstream ss(cleaned);
  std::string token;
  while (std::getline(ss, token, ',')) {
    token = trim(token);
    if (token.empty()) continue;
    std::stringstream numStream(token);
    int number;
    numStream >> number;
    if (numStream.fail())
      throw std::runtime_error("Expected number in vector: " + token);
    if (number < 0) 
      throw std::runtime_error("Invalid value in vector (must be >= 0): " + std::to_string(number));
    if (number > std::numeric_limits<short>::max()) 
      throw std::runtime_error("Value exceeds short range: " + std::to_string(number));
    vector.push_back(static_cast<short>(number));
  }
  if (static_cast<short>(vector.size()) != size) 
    throw std::runtime_error("Expected " + std::to_string(size) + " values, got " + std::to_string(vector.size()));
  return vector;
}

std::vector<std::vector<short>> LoaderMSCFLPCI::loadMatrix(std::ifstream& file, const std::string& firstLine, short rows, short cols) const {
  if (rows <= 0) 
    throw std::runtime_error("Invalid size for matrix: " + std::to_string(rows));
  std::vector<std::vector<short>> matrix;
  matrix.reserve(rows);
  std::string currentLine = firstLine;
  for (short row = 0; row < rows; ++row) {
    if (row > 0) {
      if (!getline(file, currentLine))
        throw std::runtime_error("Unexpected EOF while reading matrix row " + std::to_string(row));
    }  
    std::string cleaned = trim(currentLine);
    if (cleaned.front() == '[') cleaned = cleaned.substr(1);
    if (cleaned.front() == '|') cleaned = cleaned.substr(1);
    if (cleaned.back() == ']') cleaned.pop_back();       
    bool isLastRow = (cleaned.back() == ';');
    if (isLastRow) cleaned.pop_back();
    std::vector<short> rowData;
    rowData.reserve(cols);
    std::stringstream ss(cleaned);
    std::string token;    
    while (std::getline(ss, token, ',')) {
      token = trim(token);
      if (token.empty()) continue;
      std::stringstream numStream(token);
      int number;
      numStream >> number;
      if (numStream.fail())
        throw std::runtime_error("Expected number in matrix: " + token);
      if (number < 0)
        throw std::runtime_error("Invalid value in matrix: " + std::to_string(number));
      if (number > std::numeric_limits<short>::max())
        throw std::runtime_error("Value exceeds short range: " + std::to_string(number));  
      rowData.push_back(static_cast<short>(number));
    }
    if (static_cast<short>(rowData.size()) != cols)
      throw std::runtime_error("Row " + std::to_string(row) + " expected " + std::to_string(cols) + " values, got " + std::to_string(rowData.size()));
    matrix.push_back(std::move(rowData));    
    if (isLastRow) break;
  }
  if (static_cast<short>(matrix.size()) != rows) 
    throw std::runtime_error("Expected " + std::to_string(rows) + " rows in matrix, got " + std::to_string(matrix.size()));
  return matrix;
}

int LoaderMSCFLPCI::readInt(const std::string& value) const {
  int number = std::stoi(value);
  if (number <= 0) 
    throw std::runtime_error("Invalid value (must be > 0): " + value);
  if (number > std::numeric_limits<int>::max()) 
    throw std::runtime_error("Value exceeds int range: " + value);
  return number;
}

std::vector<ShortPair> LoaderMSCFLPCI::readVectorPairsShort(const std::string& value, short size) const {
  if (size <= 0) 
    throw std::runtime_error("Invalid size for pairs vector: " + std::to_string(size));
  std::string cleaned = trim(value);
  if (cleaned.front() == '[') cleaned = cleaned.substr(1);
  if (cleaned.front() == '|') cleaned = cleaned.substr(1);
  if (cleaned.back() == ']') cleaned.pop_back();       
  if (cleaned.back() == ';') cleaned.pop_back();
  std::vector<ShortPair> pairs;
  pairs.reserve(size);
  std::stringstream ss(cleaned);
  std::string pairToken;  
  while (std::getline(ss, pairToken, '|')) {
    pairToken = trim(pairToken);
    if (pairToken.empty()) continue;
    std::stringstream pairStream(pairToken);
    std::string num1, num2;
    if (!std::getline(pairStream, num1, ','))
      throw std::runtime_error("Invalid pair format: " + pairToken);
    if (!std::getline(pairStream, num2))
      throw std::runtime_error("Invalid pair format: " + pairToken);
    int first, second;
    std::stringstream(num1) >> first;
    std::stringstream(num2) >> second;
    if (pairStream.fail())
      throw std::runtime_error("Expected number in vector: " + pairToken);
    if (first < 0) 
      throw std::runtime_error("Invalid value in pair (must be > 0): " + std::to_string(first));
    if (second < 0) 
      throw std::runtime_error("Invalid value in pair (must be > 0): " + std::to_string(second));
    if (first > std::numeric_limits<short>::max()) 
      throw std::runtime_error("Value exceeds short range: " + std::to_string(first));
    if (second > std::numeric_limits<short>::max()) 
      throw std::runtime_error("Value exceeds short range: " + std::to_string(second));
    pairs.push_back({ static_cast<short>(first - 1), static_cast<short>(second - 1) });
    pairs.push_back({ static_cast<short>(second - 1), static_cast<short>(first - 1) });
  }
  if (static_cast<short>(pairs.size()) != (2 * size)) 
    throw std::runtime_error("Expected " + std::to_string(size) + " pairs, got " + std::to_string(pairs.size()));
  return pairs;
}
