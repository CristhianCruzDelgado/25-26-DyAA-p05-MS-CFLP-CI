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
 *   - j2 has residual capacity > 0 for store i.
 *   - Store i is compatible with all stores currently assigned to j2.
 *   - The resulting delta in supply cost is strictly negative (improvement).
 *
 * If j2 cannot absorb the full amount supplied by j1 to i, only the available
 * capacity of j2 is transferred (partial shift).
 *
 * The best feasible move across the entire neighborhood is applied per iteration.
 * The search terminates when no improving move exists in N(x).
 *
 * @param current_solution  The starting solution. The original is not modified.
 * @return A new locally optimal SolutionMSCFLPCI.
 */
SolutionMSCFLPCI* LocalSearchShift::solve(const SolutionMSCFLPCI* current_solution) const {
  const InstanceMSCFLPCI*                instance           = current_solution->getInstanceData();
  const std::vector<short>&              good               = instance->getGood();
  const std::vector<std::vector<short>>& supply_cost        = instance->getSupplyCost();
  const std::vector<ShortPair>&          incompatible_pairs = instance->getIncompatiblePairs();
  const short                            num_stores         = instance->getNumStores();

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
    short best_i      = -1;
    short best_j1     = -1;
    short best_j2     = -1;
    short best_amount = -1;

    for (short i = 0; i < num_stores; ++i) {
      const short store = i + 1;
      for (short j1 = 0; j1 < num_assigned; ++j1) {
        if (std::find(assignment[j1].begin(), assignment[j1].end(), store) == assignment[j1].end()) continue; 
        const short amount = static_cast<short>(std::round(good_supplied[i][j1] * good[i])); // amount supplied by `j1`
        if (amount == 0) continue;
        for (short j2 = 0; j2 < num_assigned; ++j2) {
          if (j1 == j2) continue;
          if (residual_capacity[j2] == 0) continue;
          if (!AlgorithmTools::verifyCompatibility(incompatible_pairs, store, assignment[j2])) continue;
          const short transferred = std::min(amount, residual_capacity[j2]);
          const float delta       = (supply_cost[i][warehouse_assigned[j2]] - supply_cost[i][warehouse_assigned[j1]]) * transferred;
          if (delta < best_delta) {
            improved    = true;
            best_delta  = delta;
            best_i      = i;
            best_j1     = j1;
            best_j2     = j2;
            best_amount = transferred;
          }
        }
      }
    }

    if (improved) {
      total_delta            += best_delta;
      const short store       = best_i + 1;
      const short full_amount = static_cast<short>(std::round(good_supplied[best_i][best_j1] * good[best_i])); // amount supplied by `j1`
      const float fraction    = static_cast<float>(best_amount) / good[best_i];

      if (best_amount == full_amount) {
        // Full shift: remove store from j1
        assignment[best_j1].erase(std::find(assignment[best_j1].begin(), assignment[best_j1].end(), store));
        good_supplied[best_i][best_j1] = 0.0f;
      } else {
        // Partial shift: reduce fraction in j1
        good_supplied[best_i][best_j1] -= fraction;
      }

      if (std::find(assignment[best_j2].begin(), assignment[best_j2].end(), store) == assignment[best_j2].end())
        assignment[best_j2].push_back(store);

      good_supplied[best_i][best_j2] += fraction;
      residual_capacity[best_j1]     += best_amount;
      residual_capacity[best_j2]     -= best_amount;
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
  if (epsilon > 1e-3f) throw std::runtime_error("There is no coincidence between deltas. LocalSearchShift::solve");
  return solution;
}
