#include "../include/table8.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"
 
Table8::Table8(
  const Solution* solution
) :
  solution_(solution) {}
 
void Table8::displayOnConsole() const {
  std::cout << "Table 8: Objective Value\n";
  if (solution_ == nullptr) throw std::invalid_argument("Solution not set. Table8::displayOnConsole");
  const SolutionMSCFLPCI* solution = dynamic_cast<const SolutionMSCFLPCI*>(solution_);
  if (solution == nullptr) throw std::invalid_argument("Invalid solution type. Table8::displayOnConsole");
 
  std::cout << BOLD << std::setprecision(2) << "\n"
            << std::setw(20) << "Objective Values:"
            << std::setw(14) << "Costs"
            << "\n" << RESET
            << std::setw(20) << "Fixed Costs Sum:"
            << std::setw(14) << solution->getFixedCostsSum() << "\n"
            << std::setw(20) << "Supply Costs Sum:"
            << std::setw(14) << solution->getSupplyCostsSum() << "\n"
            << BOLD
            << std::setw(20) << "Objective Value:"
            << std::setw(14) << solution->getObjectiveValue() << "\n"
            << std::setw(20) << "Execution Time:"
            << std::setw(14) << solution->getTime() << " seconds\n"
            << "\n\n" << RESET;
}
