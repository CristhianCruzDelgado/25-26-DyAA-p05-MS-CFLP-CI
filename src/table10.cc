#include "../include/table10.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"
 
Table10::Table10(
  const char* input_file,
  const Solution* solution
) :
  input_file_(input_file),
  solution_(solution) {}
 
void Table10::displayOnConsole() const {
  std::cout << "Table 10: report results\n";
  if (solution_ == nullptr) throw std::invalid_argument("Solution not set. Table10::displayOnConsole");
  const SolutionMSCFLPCI* solution = dynamic_cast<const SolutionMSCFLPCI*>(solution_);
  if (solution == nullptr) throw std::invalid_argument("Invalid solution type. Table10::displayOnConsole");
  std::string filename = std::filesystem::path(input_file_).filename().string();
  std::cout << BOLD << std::setprecision(2) << "\n"
            << std::setw(12) << "wlpXX.dzn"
            << std::setw(20) << "Open Warehouses"
            << std::setw(20) << "Fixed Costs Sum"
            << std::setw(20) << "Supply Costs Sum"
            << std::setw(20) << "Objective Value"
            << std::setw(20) << "Incompatibilities"
            << std::setw(20) << "Execution Time"
            << "\n" << RESET
            << std::setw(12) << filename
            << std::setw(20) << solution->getNumWarehousesAssigned()
            << std::setw(20) << solution->getFixedCostsSum()
            << std::setw(20) << solution->getSupplyCostsSum()
            << std::setw(20) << solution->getObjectiveValue()
            << std::setw(17) << "0/" <<  solution->getInstanceData()->getNumIncompatibilities()
            << std::setw(20) << solution->getTime()
            << "\n\n";
}
