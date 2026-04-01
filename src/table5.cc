#include "../include/table5.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"

Table5::Table5(
  const Solution* solution
) :
  solution_(solution) {}

void Table5::displayOnConsole() const {
  std::cout << "Table 5: Assignment Data\n";
  const SolutionMS_CFLP_CI* solution = dynamic_cast<const SolutionMS_CFLP_CI*>(solution_);
  if (solution == nullptr) throw std::invalid_argument("Invalid solution type. Table5::displayOnConsole");
  const InstanceMS_CFLP_CI* instance = solution->getInstanceData();
  short num_stores = instance->getNumStores();
  short num_assigned = solution->getNumWarehousesAssigned();
  const std::vector<short>& good = instance->getGood();
  const std::vector<short>& warehouse_assigned = solution->getWarehouseAssigned();
  const std::vector<std::vector<float>>& good_supplied = solution->getGoodSupplied();

  std::cout << BOLD << "\nStores & Warehouses:\n"
            << std::setw(8) << "Store"
            << std::setw(10) << "Warehouse"
            << std::setw(6) << "Good"
            << std::setw(12) << "Percentage"
            << "\n" << RESET;
  for (short i = 0; i < num_stores; ++i) {
    for (short j = 0; j < num_assigned; ++j) {
      if (good_supplied[i][j] > 0.0f) {
        std::cout << std::setw(8) << i + 1
                  << std::setw(10) << warehouse_assigned[j] + 1
                  << std::setw(6) << good[i]
                  << std::setw(9) << std::fixed << std::setprecision(2)
                  << good_supplied[i][j] << "\n";
      }
    }
  }
  std::cout << "\n";
}