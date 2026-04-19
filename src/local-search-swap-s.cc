#include "../include/local-search-swap-s.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"

/**
 * @brief Performs a best-improvement local search using the Store-Swap neighborhood.
 *
 * Explores the full neighborhood N(x) = { (i1, j1, i2, j2) | store i1 is assigned
 * to j1, store i2 is assigned to j2, j1 ≠ j2 }, selecting the move with the
 * greatest improvement in supply cost.
 *
 * A move (i1, j1, i2, j2) is feasible if and only if:
 *   - j1 has enough residual capacity to absorb the net difference (amount2 - amount1).
 *   - j2 has enough residual capacity to absorb the net difference (amount1 - amount2).
 *   - store i1 is compatible with all stores currently assigned to j2 except i2.
 *   - store i2 is compatible with all stores currently assigned to j1 except i1.
 *   - The resulting delta in supply cost is strictly negative (improvement).
 *
 * The best feasible move across the entire neighborhood is applied per iteration.
 * The search terminates when no improving move exists in N(x).
 *
 * @param current_solution  The starting solution. The original is not modified.
 * @return A new locally optimal SolutionMSCFLPCI.
 */
SolutionMSCFLPCI* LocalSearchSwapS::solve(const SolutionMSCFLPCI* current_solution) const {
  const InstanceMSCFLPCI*                instance           = current_solution->getInstanceData();
  const std::vector<short>&              good               = instance->getGood();
  const std::vector<std::vector<short>>& supply_cost        = instance->getSupplyCost();
  const std::vector<ShortPair>&          incompatible_pairs = instance->getIncompatiblePairs();

  const short                     num_assigned       = current_solution->getNumWarehousesAssigned();
  std::vector<short>              warehouse_assigned = current_solution->getWarehouseAssigned();
  std::vector<std::vector<short>> assignment         = current_solution->getAssignment();
  std::vector<std::vector<float>> good_supplied      = current_solution->getGoodSupplied();
  std::vector<short>              residual_capacity  = current_solution->getResidualCapacity();
  std::vector<short>              residual_good      = current_solution->getResidualGood();

  bool improved     = true;
  float total_delta = 0.0f;
  while (improved) {
    improved = false;

    float best_delta  = 0.0f;
    short best_j1     = -1;
    short best_j2     = -1;
    short best_i1_idx = -1;
    short best_i2_idx = -1;

    // upper triangular matrix for `j`
    for (short j1 = 0; j1 < num_assigned; ++j1) {
      for (short i1_idx = 0; i1_idx < (short)assignment[j1].size(); ++i1_idx) {
        const short store1  = assignment[j1][i1_idx];
        const short i1      = store1 - 1;
        const short amount1 = static_cast<short>(std::round(good_supplied[i1][j1] * good[i1])); // amount supplied by `j1` to `i1`

        for (short j2 = j1 + 1; j2 < num_assigned; ++j2) {
          for (short i2_idx = 0; i2_idx < (short)assignment[j2].size(); ++i2_idx) {
            const short store2  = assignment[j2][i2_idx];
            const short i2      = store2 - 1;
            const short amount2 = static_cast<short>(std::round(good_supplied[i2][j2] * good[i2])); // amount supplied by `j2`to `i2`

            if (residual_capacity[j1] < amount2 - amount1) continue;
            if (residual_capacity[j2] < amount1 - amount2) continue;

            std::vector<short> j2_without_i2;
            for (short x : assignment[j2]) if (x != store2) j2_without_i2.push_back(x);
            if (!AlgorithmTools::verifyCompatibility(incompatible_pairs, store1, j2_without_i2)) continue;

            std::vector<short> j1_without_i1;
            for (short x : assignment[j1]) if (x != store1) j1_without_i1.push_back(x);
            if (!AlgorithmTools::verifyCompatibility(incompatible_pairs, store2, j1_without_i1)) continue;

            const float delta =
              (supply_cost[i1][warehouse_assigned[j2]] - supply_cost[i1][warehouse_assigned[j1]]) * amount1 +
              (supply_cost[i2][warehouse_assigned[j1]] - supply_cost[i2][warehouse_assigned[j2]]) * amount2;
            if (delta < best_delta) {
              improved    = true;
              best_delta  = delta;
              best_j1     = j1;     
              best_j2     = j2;
              best_i1_idx = i1_idx; 
              best_i2_idx = i2_idx;
            }
          }
        }
      }
    }

    if (improved) {
      total_delta        += best_delta;
      const short store1  = assignment[best_j1][best_i1_idx];
      const short store2  = assignment[best_j2][best_i2_idx];
      const short i1      = store1 - 1;
      const short i2      = store2 - 1;
      const short amount1 = static_cast<short>(std::round(good_supplied[i1][best_j1] * good[i1])); // amount supplied by `j1`
      const short amount2 = static_cast<short>(std::round(good_supplied[i2][best_j2] * good[i2])); // amount supplied by `j2`

      assignment[best_j1].erase(assignment[best_j1].begin() + best_i1_idx);
      assignment[best_j2].erase(assignment[best_j2].begin() + best_i2_idx);
      assignment[best_j1].push_back(store2);
      assignment[best_j2].push_back(store1);

      good_supplied[i1][best_j2] += good_supplied[i1][best_j1];
      good_supplied[i1][best_j1]  = 0.0f;
      good_supplied[i2][best_j1] += good_supplied[i2][best_j2];
      good_supplied[i2][best_j2]  = 0.0f;

      residual_capacity[best_j1] += amount1 - amount2;
      residual_capacity[best_j2] += amount2 - amount1;
    }
  }

  SolutionMSCFLPCI* solution = new SolutionMSCFLPCI(
    instance,
    std::move(warehouse_assigned),
    std::move(assignment),
    std::move(good_supplied),
    std::move(residual_capacity),
    std::move(residual_good)
  );
  float epsilon = static_cast<float>(current_solution->getObjectiveValue() + total_delta - solution->getObjectiveValue());
  if (epsilon > 1e-3f) throw std::runtime_error("There is no coincidence between deltas. LocalSearchSwapS::solve");
  return solution;
}
