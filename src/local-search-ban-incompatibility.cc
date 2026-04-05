#include "../include/local-search-ban-incompatibility.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"

/**
 * @brief Performs a best-improvement local search using the Incompatibility-Ban neighborhood.
 *
 * Each iteration identifies the open warehouse with the highest total supply cost
 * (worst_warehouse) and within it the store that causes the most active incompatibility
 * conflicts (blocker_store). It then evaluates all alternative open warehouses and selects
 * the one that yields the greatest improvement in supply cost for relocating blocker_store.
 *
 * A move (blocker_store, worst_warehouse, j2) is feasible if and only if:
 *   - j2 has enough residual capacity to absorb the amount supplied to blocker_store.
 *   - blocker_store is compatible with all stores currently assigned to j2.
 *   - The resulting delta in supply cost is strictly negative (improvement).
 *
 * Unlike a pure Shift, the candidate store is selected by incompatibility pressure rather
 * than exhaustive enumeration, making this neighborhood particularly effective in instances
 * with dense incompatibility constraints.
 *
 * The best feasible destination across all open warehouses is applied per iteration.
 * The search terminates when no improving move exists.
 *
 * @param current_solution  The starting solution. Its data is copied; the
 *                          original is not modified.
 * @return A new locally optimal SolutionMSCFLPCI. The caller takes ownership.
 */
SolutionMSCFLPCI* LocalSearchBanIncompatibility::solve(SolutionMSCFLPCI* current_solution) const {
  const InstanceMSCFLPCI* instance = current_solution->getInstanceData();
  const std::vector<short>& good = instance->getGood();
  const std::vector<std::vector<short>>& supply_cost = instance->getSupplyCost();
  const std::vector<ShortPair>& incompatible_pairs = instance->getIncompatiblePairs();
  const short num_stores = instance->getNumStores();
  const short num_assigned = current_solution->getNumWarehousesAssigned();
  std::vector<short> warehouse_assigned = current_solution->getWarehouseAssigned();
  std::vector<std::vector<short>> assignment = current_solution->getAssignment();
  std::vector<std::vector<float>> good_supplied = current_solution->getGoodSupplied();
  std::vector<short> residual_capacity = current_solution->getResidualCapacity();
  std::vector<short> residual_good = current_solution->getResidualGood();

  bool improved = true;
  while (improved) {
    improved = false;

    short worst_warehouse = -1;
    float worst_supply_cost = -1.0;
    for (short j1 = 0; j1 < num_assigned; ++j1) {
      float total = 0.0;
      for (short store : assignment[j1]) {
        short i = store - 1;
        short amount = static_cast<short>(good_supplied[i][j1] * good[i]);
        total += supply_cost[i][warehouse_assigned[j1]] * amount;
      }
      if (total > worst_supply_cost) {
        worst_supply_cost = total;
        worst_warehouse = j1;
      }
    }
    if (worst_warehouse == -1 || assignment[worst_warehouse].empty()) continue;
    short blocker_store = -1;
    short blocker_i = -1;
    short max_blocks = -1;
    for (short store : assignment[worst_warehouse]) {
      short i = store - 1;
      short blocks = 0;
      for (const ShortPair& pair : incompatible_pairs) {
        if (pair.first == store) {
          for (short other : assignment[worst_warehouse])
            if (other == pair.second) ++blocks;
        }
      }
      if (blocks > max_blocks) {
        max_blocks = blocks;
        blocker_store = store;
        blocker_i = i;
      }
    }
    if (blocker_store == -1) continue;
    short amount = static_cast<short>(good_supplied[blocker_i][worst_warehouse] * good[blocker_i]);
    float best_delta = 0.0;
    short best_j2 = -1;
    for (short j2 = 0; j2 < num_assigned; ++j2) {
      if (j2 == worst_warehouse) continue;
      if (residual_capacity[j2] < amount) continue;
      std::vector<short> j2_without_blocker;
      for (short x : assignment[j2]) if (x != blocker_store) j2_without_blocker.push_back(x);
      if (!AlgorithmTools::verifyCompatibility(incompatible_pairs, blocker_store, j2_without_blocker)) continue;
      float delta = (supply_cost[blocker_i][warehouse_assigned[j2]] - supply_cost[blocker_i][warehouse_assigned[worst_warehouse]]) * amount;
      if (delta < best_delta) {
        best_delta = delta;
        best_j2 = j2;
      }
    }
    if (best_j2 != -1) {
      auto it = std::find(assignment[worst_warehouse].begin(), assignment[worst_warehouse].end(), blocker_store);
      assignment[worst_warehouse].erase(it);
      assignment[best_j2].push_back(blocker_store);
      good_supplied[blocker_i][best_j2] += good_supplied[blocker_i][worst_warehouse];
      good_supplied[blocker_i][worst_warehouse] = 0.0;
      residual_capacity[worst_warehouse] += amount;
      residual_capacity[best_j2] -= amount;
      improved = true;
    }
  }

  return new SolutionMSCFLPCI(
    instance,
    std::move(warehouse_assigned),
    std::move(assignment),
    std::move(good_supplied),
    std::move(residual_capacity),
    std::move(residual_good));
}