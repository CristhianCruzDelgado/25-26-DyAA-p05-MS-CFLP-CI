#include "../include/table4.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"
 
Table4::Table4(
  const Solution* solution
) :
  solution_(solution) {}
 
void Table4::displayOnConsole() const {
  std::cout << "Table 4: Fixed Costs\n";
  const SolutionMSCFLPCI* solution = dynamic_cast<const SolutionMSCFLPCI*>(solution_);
  if (solution == nullptr) throw std::invalid_argument("Invalid solution type. Table4::displayOnConsole");
  const InstanceMSCFLPCI* instance = solution->getInstanceData();
  short num_warehouses = instance->getNumWarehouses();
  const std::vector<short>& warehouse_assigned = solution->getWarehouseAssigned();
  const std::vector<short>& fixed_cost = instance->getFixedCost();
 
  std::cout << BOLD << "\nWarehouses:\n"
            << std::setw(6) << "ID"
            << std::setw(10) << "State"
            << std::setw(12) << "Fixed-Cost"
            << "\n" << RESET;
  std::vector<bool> is_open(num_warehouses, false);
  for (short j : warehouse_assigned) is_open[j] = true;
  for (short j = 0; j < num_warehouses; ++j) {
    std::cout << std::setw(6) << j + 1
              << std::setw(10) << (is_open[j] ? "Open" : "Closed")
              << std::setw(12) << fixed_cost[j] << "\n";
  }
  std::cout << BOLD
            << std::setw(16) << "Total Fixed Costs:"
            << std::setw(10) << solution->getFixedCostsSum()
            << "\n\n" << RESET;
}