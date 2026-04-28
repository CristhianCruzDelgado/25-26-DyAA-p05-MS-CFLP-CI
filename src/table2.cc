#include "../include/table2.h"
#include "../include/instance-MS-CFLP-CI.h"
 
Table2::Table2(
  const Instance* instance
) :
  instance_(instance) {}
 
void Table2::displayOnConsole() const {
  std::cout << "Table 2: Supply Costs\n\n";
  if (instance_ == nullptr) throw std::invalid_argument("Instance not set. Table2::displayOnConsole");
  const InstanceMSCFLPCI* instance = dynamic_cast<const InstanceMSCFLPCI*>(instance_);
  if (instance == nullptr) throw std::invalid_argument("Invalid instance type. Table2::displayOnConsole");
  short num_stores = instance->getNumStores();
  short num_warehouses = instance->getNumWarehouses();
  const std::vector<std::vector<short>>& supply_cost = instance->getSupplyCost();
 
  std::cout << BOLD << "Supply Costs:\n Stores x Warehouses 1 .. " << num_warehouses << "\n" << RESET;
  std::cout << "Store 1: ";
  for (short j = 0; j < num_warehouses; ++j) std::cout << supply_cost[0][j] << " ";
  std::cout << "\nStore 2: ";
  for (short j = 0; j < num_warehouses; ++j) std::cout << supply_cost[1][j] << " ";
  std::cout << "\n.\n.\n.\nStore " << num_stores - 1 << ": ";
  for (short j = 0; j < num_warehouses; ++j) std::cout << supply_cost[num_stores - 2][j] << " ";
  std::cout << "\nStore " << num_stores << ": ";
  for (short j = 0; j < num_warehouses; ++j) std::cout << supply_cost[num_stores - 1][j] << " ";
  std::cout << "\n";
}
