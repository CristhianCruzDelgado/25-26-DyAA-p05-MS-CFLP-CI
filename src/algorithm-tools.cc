#include "algorithm.tools.h"

// Position i of the returned vector contains the index of the warehouse with the i-th lowest fixed cost
std::vector<short> AlgorithmTools::sortFixedCostAscending(const std::vector<short>& v, const short& size) {
  std::vector<short> indices(size);
  for (short i = 0; i < size; ++i)
    indices[i] = i;
  std::sort(indices.begin(), indices.end(), [&](short a, short b) {
    return v[a] < v[b];
  });
  return indices;
}

// Position i of the returned vector contains the index of the warehouse with the i-th lowest supply cost for store i
std::vector<short> AlgorithmTools::sortSupplyCostAscending(std::vector<short> warehouse_assigned, const short& size, const std::vector<short>& v) {
  std::vector<short> indices(size);
  for (short i = 0; i < size; ++i)
    indices[i] = i;
  std::sort(indices.begin(), indices.end(), [&](short a, short b) {
    return v[warehouse_assigned[a]] < v[warehouse_assigned[b]];
  });
  return indices;
}

bool AlgorithmTools::verifyCompatibility(const std::vector<ShortPair>& incompatible_pairs, const short& store, const std::vector<short>& assingments) {
  if (assingments.empty()) return true;
  for (const ShortPair& pair : incompatible_pairs) 
    if (pair.first == store) 
      for (short assigned_store : assingments) 
        if (assigned_store == pair.second) 
          return false;
  return true;
}