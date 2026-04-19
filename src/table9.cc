#include "../include/table9.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"
 
Table9::Table9(
  const char* input_file,
  const Solution* solution
) :
  input_file_(input_file),
  solution_(solution) {}
 
void Table9::displayOnConsole() const {
  std::cout << "Table 9: statement results\n";
  const SolutionMSCFLPCI* solution = dynamic_cast<const SolutionMSCFLPCI*>(solution_);
  if (solution == nullptr) throw std::invalid_argument("Invalid solution type. Table9::displayOnConsole");
  std::string filename = std::filesystem::path(input_file_).filename().string();
  std::cout << BOLD << "\n"
            << std::setw(12) << "wlpXX.dzn"
            << std::setw(11) << "Stores"
            << std::setw(13) << "Warehouses"
            << std::setw(20) << "Objective Value"
            << "\n" << RESET
            << std::setw(12) << filename
            << std::setw(11) << solution->getInstanceData()->getNumStores()
            << std::setw(13) << solution->getInstanceData()->getNumWarehouses()
            << std::setw(20) << solution->getObjectiveValue()
            << "\n\n";
}