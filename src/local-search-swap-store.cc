#include "../include/local-search-swap-store.h"
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
  if (current_solution == nullptr) throw std::invalid_argument("Invalid solution parameter. LocalSearchSwapS::solve");
  const InstanceMSCFLPCI*                instance           = current_solution->getInstanceData();
  if (instance == nullptr) throw std::invalid_argument("Invalid solution parameter. LocalSearchSwapS::solve");
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

    float best_delta = 0.0f;
    short best_kj1   = -1;
    short best_kj2   = -1;
    short best_ki1   = -1;
    short best_ki2   = -1;

    // upper triangular matrix for `j`
    for (short kj1 = 0; kj1 < num_assigned; ++kj1) {
      short j1 = warehouse_assigned[kj1];
      for (short ki1 = 0; ki1 < assignment[kj1].size(); ++ki1) {
        const short i1  = assignment[kj1][ki1];
        const short amount1 = static_cast<short>(std::round(good_supplied[i1][kj1] * good[i1]));

        for (short kj2 = kj1 + 1; kj2 < num_assigned; ++kj2) {
          short j2 = warehouse_assigned[kj2];
          for (short ki2 = 0; ki2 < assignment[kj2].size(); ++ki2) {
            const short i2  = assignment[kj2][ki2];
            const short amount2 = static_cast<short>(std::round(good_supplied[i2][kj2] * good[i2]));

            if (residual_capacity[kj1] < amount2 - amount1) continue;
            if (residual_capacity[kj2] < amount1 - amount2) continue;

            std::vector<short> kj2_without_ki2 = assignment[kj2];
            kj2_without_ki2.erase(kj2_without_ki2.begin() + ki2);
            if (!AlgorithmTools::verifyCompatibility(incompatible_pairs, i1, kj2_without_ki2)) continue;

            std::vector<short> kj1_without_ki1 = assignment[kj1];
            kj1_without_ki1.erase(kj1_without_ki1.begin() + ki1);
            if (!AlgorithmTools::verifyCompatibility(incompatible_pairs, i2, kj1_without_ki1)) continue;

            const float delta =
              (supply_cost[i1][j2] - supply_cost[i1][j1]) * amount1 +
              (supply_cost[i2][j1] - supply_cost[i2][j2]) * amount2;

            if (delta < best_delta) {
              improved   = true;
              best_delta = delta;
              best_kj1   = kj1;     
              best_kj2   = kj2;
              best_ki1   = ki1; 
              best_ki2   = ki2;
            }
          }
        }
      }
    }

    if (improved) {
        const short i1 = assignment[best_kj1][best_ki1];
        const short i2 = assignment[best_kj2][best_ki2];
        
        const float fraction1 = good_supplied[i1][best_kj1];
        const float fraction2 = good_supplied[i2][best_kj2];

        assignment[best_kj1][best_ki1] = i2;
        assignment[best_kj2][best_ki2] = i1;

        good_supplied[i1][best_kj2] += fraction1;
        good_supplied[i1][best_kj1] = 0.0f;
        good_supplied[i2][best_kj1] += fraction2;
        good_supplied[i2][best_kj2] = 0.0f;

        float amount1 = fraction1 * good[i1];
        float amount2 = fraction2 * good[i2];
        
        residual_capacity[best_kj1] += static_cast<short>(std::round(amount1 - amount2));
        residual_capacity[best_kj2] += static_cast<short>(std::round(amount2 - amount1));

        total_delta += best_delta;
    }
  }

  SolutionMSCFLPCI* solution = new SolutionMSCFLPCI(
    instance, warehouse_assigned, assignment, good_supplied, 
    residual_capacity, residual_good
  );
  float epsilon = static_cast<float>(current_solution->getObjectiveValue() + total_delta - solution->getObjectiveValue());
  // if (epsilon > 1) throw std::runtime_error("There is no coincidence between deltas. LocalSearchSwapS::solve");
  return solution;
}
