#include "../include/table1.h"
#include "../include/instance-MS-CFLP-CI.h"

Table1::Table1(
  const Instance* instance
) :
  instance_(instance) {}

void Table1::displayOnConsole() const {
  std::cout << "Table 1: Instance Data\n";
  const InstanceMS_CFLP_CI* instance = dynamic_cast<const InstanceMS_CFLP_CI*>(instance_);
  if (instance == nullptr) throw std::invalid_argument("Invalid instance type. Table1::displayOnConsole");
  short num_warehouses = instance->getNumWarehouses();
  short num_stores = instance->getNumStores();
  const std::vector<short>& capacity = instance->getCapacity();
  const std::vector<short>& fixed_cost = instance->getFixedCost();
  const std::vector<short>& good = instance->getGood();
  short num_incompatibilities = instance->getNumIncompatibilities();
  const std::vector<ShortPair>& incompatible_pairs = instance->getIncompatiblePairs();

  std::cout << BOLD << "\nWarehouses:\n"
            << std::setw(6) << "ID"
            << std::setw(10) << "Capacity"
            << std::setw(12) << "Fixed-Cost"
            << "\n" << RESET;
  for (short j = 0; j < num_warehouses; ++j) {
    std::cout << std::setw(6) << j + 1
              << std::setw(10) << capacity[j]
              << std::setw(12) << fixed_cost[j] << "\n";
  }
  std::cout << BOLD << "\nStores:\n"
            << std::setw(6) << "ID"
            << std::setw(8) << "Goods"
            << std::setw(20) << "Incompatibilities"
            << "\n" << RESET;
  for (short i = 0; i < num_stores; ++i) {
    std::cout << std::setw(6) << i + 1
              << std::setw(8) << good[i]
              << std::setw(3) << " ";
    for (short p = 0; p < num_incompatibilities; ++p)
      if (incompatible_pairs[p].first == (i + 1))
        std::cout << incompatible_pairs[p].second << " ";
    std::cout << "\n";
  }
  std::cout << "\n";
}