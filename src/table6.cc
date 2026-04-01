#include "../include/table6.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"

Table6::Table6(
  const Solution* solution
) :
  solution_(solution) {}

void Table6::displayOnConsole() const {
  std::cout << "Table 6: Verify Restrictions\n";
  const SolutionMS_CFLP_CI* solution = dynamic_cast<const SolutionMS_CFLP_CI*>(solution_);
  if (solution == nullptr) throw std::invalid_argument("Invalid solution type. Table6::displayOnConsole");
  const InstanceMS_CFLP_CI* instance = solution->getInstanceData();
  const short num_warehouses = instance->getNumWarehouses();
  const short num_incompatibilities = instance->getNumIncompatibilities();
  const short num_assigned = solution->getNumWarehousesAssigned();
  const std::vector<short>& capacity = instance->getCapacity();
  const std::vector<short>& warehouse_assigned = solution->getWarehouseAssigned();
  const std::vector<short>& residual_capacity = solution->getResidualCapacity();
  const std::vector<std::vector<short>>& assignment = solution->getAssignment();
  const std::vector<ShortPair>& incompatible_pairs = instance->getIncompatiblePairs();

  std::cout << BOLD << "\nWarehouses:\n"
            << std::setw(6) << "ID"
            << std::setw(14) << "Good-Supplied"
            << std::setw(9) << "Capacity"
            << std::setw(9) << "State"
            << "\n" << RESET;
  for (short j = 0; j < num_assigned; ++j) {
    std::cout << std::setw(6) << warehouse_assigned[j] + 1
              << std::setw(14) << capacity[warehouse_assigned[j]] - residual_capacity[j]
              << std::setw(9) << capacity[warehouse_assigned[j]]
              << std::setw(11) << (residual_capacity[j] == 0 ? "✓Limit" : "✓OK") << "\n";
  }
  std::cout << BOLD << "\nIncompatibility Pairs:\n"
            << std::setw(14) << "First-Store"
            << std::setw(14) << "Second-Store"
            << std::setw(12) << "State"
            << "\n" << RESET;
  for (short p = 0; p < num_incompatibilities; ++p) {
    short s1 = incompatible_pairs[p].first;
    short s2 = incompatible_pairs[p].second;
    bool  violation = false;
    for (short j = 0; j < num_assigned; ++j) {
      bool has_s1 = false, has_s2 = false;
      for (short s : assignment[j]) {
        if (s == s1) has_s1 = true;
        if (s == s2) has_s2 = true;
      }
      if (has_s1 && has_s2) { violation = true; break; }
    }
    if (violation) {
      std::cout << std::setw(14) << s1
                << std::setw(14) << s2
                << std::setw(9) << "✗Violation\n";
    }
  }
  std::cout << "\n";
}