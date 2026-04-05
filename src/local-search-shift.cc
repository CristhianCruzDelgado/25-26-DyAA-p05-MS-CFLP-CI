#include "../include/local-search-shift.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"

/**
 * @brief Performs a best-improvement local search using the Shift neighborhood.
 *
 * Explores the full neighborhood N(x) = { (i, j1, j2) | store i is assigned to j1,
 * j2 is an open warehouse where i is not assigned, j1 ≠ j2 }, selecting the move
 * with the greatest improvement in supply cost.
 *
 * A move (i, j1, j2) is feasible if and only if:
 *   - j2 has enough residual capacity to absorb the amount supplied by j1 to i.
 *   - Store i is compatible with all stores currently assigned to j2.
 *   - The resulting delta in supply cost is strictly negative (improvement).
 *
 * The best feasible move across the entire neighborhood is applied per iteration.
 * The search terminates when no improving move exists in N(x).
 *
 * @param current_solution  The starting solution. Its data is copied; the
 *                          original is not modified.
 * @return A new locally optimal SolutionMSCFLPCI. The caller takes ownership.
 */
SolutionMSCFLPCI* LocalSearchShift::solve(SolutionMSCFLPCI* current_solution) const {
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

    float best_delta = 0.0;
    short best_i = -1;
    short best_j1 = -1;
    short best_j2 = -1;

    for (short i = 0; i < num_stores; ++i) {
      const short store = i + 1;

      for (short j1 = 0; j1 < num_assigned; ++j1) {
        auto it = std::find(assignment[j1].begin(), assignment[j1].end(), store);
        if (it == assignment[j1].end()) continue;

        for (short j2 = 0; j2 < num_assigned; ++j2) {
          if (j1 == j2) continue;
          if (!AlgorithmTools::verifyCompatibility(incompatible_pairs, store, assignment[j2])) continue;

          const short amount = static_cast<short>(good_supplied[i][j1] * good[i]);
          if (residual_capacity[j2] < amount) continue;

          const float delta = (supply_cost[i][warehouse_assigned[j2]] - supply_cost[i][warehouse_assigned[j1]]) * amount;
          if (delta < best_delta) {
            best_delta = delta;
            best_i = i; 
            best_j1 = j1; 
            best_j2 = j2;
          }
        }
      }
    }
    if (best_i != -1) {
      const short store = best_i + 1;
      const short amount = static_cast<short>(good_supplied[best_i][best_j1] * good[best_i]);
      auto it = std::find(assignment[best_j1].begin(), assignment[best_j1].end(), store);
      assignment[best_j1].erase(it);
      assignment[best_j2].push_back(store);
      good_supplied[best_i][best_j2] += good_supplied[best_i][best_j1];
      good_supplied[best_i][best_j1]  = 0.0;
      residual_capacity[best_j1] += amount;
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
