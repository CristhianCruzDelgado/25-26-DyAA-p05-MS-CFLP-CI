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
 * capacity of j2 is supply_amount (partial shift).
 *
 * The best feasible move across the entire neighborhood is applied per iteration.
 * The search terminates when no improving move exists in N(x).
 *
 * @param current_solution  The starting solution. The original is not modified.
 * @return A new locally optimal SolutionMSCFLPCI.
 */
SolutionMSCFLPCI* LocalSearchShift::solve(const SolutionMSCFLPCI* current_solution) const {
  if (current_solution == nullptr) throw std::invalid_argument("Invalid solution parameter. LocalSearchShift::solve");
  const InstanceMSCFLPCI*                instance           = current_solution->getInstanceData();
  if (instance == nullptr) throw std::invalid_argument("Invalid solution parameter. LocalSearchShift::solve");
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
    short best_amount = 0;

    for (short i = 0; i < num_stores; ++i) {

      for (short j1 = 0; j1 < num_assigned; ++j1) {
        if (good_supplied[i][j1] == 0) continue; 
        const short amount = static_cast<short>(std::round(good_supplied[i][j1] * good[i]));
        if (amount == 0) continue;

        for (short j2 = 0; j2 < num_assigned; ++j2) {
          if (j1 == j2) continue;
          if (residual_capacity[j2] == 0) continue;
          if (!AlgorithmTools::verifyCompatibility(incompatible_pairs, i, assignment[j2])) continue;
          const short supply_amount = std::min(amount, residual_capacity[j2]);
          if (supply_amount == 0) continue;
          const float delta = (supply_cost[i][warehouse_assigned[j2]] - supply_cost[i][warehouse_assigned[j1]]) * supply_amount;

          if (delta < best_delta) {
            improved    = true;
            best_delta  = delta;
            best_i      = i;
            best_j1     = j1;
            best_j2     = j2;
            best_amount = supply_amount;
          }
        }
      }
    }

    if (improved) {
      total_delta += best_delta;
      const short full_amount = static_cast<short>(std::round(good_supplied[best_i][best_j1] * good[best_i]));
      const float fraction = best_amount / (float)good[best_i];

      if (std::abs(fraction - full_amount) < 1e-6f) {
        // Full shift: remove store from j1
        assignment[best_j1].erase(std::find(assignment[best_j1].begin(), assignment[best_j1].end(), best_i));
        good_supplied[best_i][best_j1] = 0.0f;
      } else {
        // Partial shift: reduce fraction in j1
        good_supplied[best_i][best_j1] -= fraction;
      }

      if (good_supplied[best_i][best_j2] < 1e-6f) {
        assignment[best_j2].push_back(best_i);
      }

      good_supplied[best_i][best_j2] += fraction;
      residual_capacity[best_j1] += best_amount;
      residual_capacity[best_j2] -= best_amount;
    }
  }

  SolutionMSCFLPCI* solution = new SolutionMSCFLPCI(
    instance, warehouse_assigned, assignment, good_supplied,    
    residual_capacity, residual_good
  );
  float epsilon = static_cast<float>(current_solution->getObjectiveValue() + total_delta - solution->getObjectiveValue());
  // if (epsilon > 1) throw std::runtime_error("There is no coincidence between deltas. LocalSearchShift::solve");
  return solution;
}
