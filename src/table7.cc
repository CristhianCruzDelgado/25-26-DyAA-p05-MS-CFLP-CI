#include "../include/table7.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"
 
Table7::Table7(
  const Solution* solution
) :
  solution_(solution) {}
 
void Table7::displayOnConsole() const {
  std::cout << "Table 7: Supply Costs\n";
  if (solution_ == nullptr) throw std::invalid_argument("Solution not set. Table7::displayOnConsole");
  const SolutionMSCFLPCI* solution = dynamic_cast<const SolutionMSCFLPCI*>(solution_);
  if (solution == nullptr) throw std::invalid_argument("Invalid solution type. Table7::displayOnConsole");
  const InstanceMSCFLPCI* instance = solution->getInstanceData();
  const short num_stores = instance->getNumStores();
  const short num_assigned = solution->getNumWarehousesAssigned();
  const std::vector<short>& good = instance->getGood();
  const std::vector<short>& warehouse_assigned = solution->getWarehouseAssigned();
  const std::vector<std::vector<short>>& supply_cost = instance->getSupplyCost();
  const std::vector<std::vector<float>>& good_supplied = solution->getGoodSupplied();
 
  std::cout << BOLD << "\nWarehouses & Stores:\n"
            << std::setw(10) << "Warehouse"
            << std::setw(6) << "Store"
            << std::setw(14) << "Good-Supplied"
            << std::setw(13) << "Supply-Cost"
            << std::setw(10) << "Subtotal"
            << "\n" << RESET;
  for (short i = 0; i < num_stores; ++i) {
    for (short j = 0; j < num_assigned; ++j) {
      if (good_supplied[i][j] > 0.0f) {
        float goods_qty    = good[i] * good_supplied[i][j];
        short unit_cost    = supply_cost[i][warehouse_assigned[j]];
        float subtotal     = goods_qty * unit_cost;
        std::cout << std::setw(10) << warehouse_assigned[j] + 1
                  << std::setw(6)  << i + 1
                  << std::setw(14) << goods_qty
                  << std::setw(13) << unit_cost
                  << std::setw(10) << subtotal << "\n";
      }
    }
  }
 
  std::cout << BOLD
            << std::setw(32) << "Total Supply Costs:"
            << std::setw(14) << solution->getSupplyCostsSum()
            << "\n\n" << RESET;
}
