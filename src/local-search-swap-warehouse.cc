#include "../include/local-search-swap-warehouse.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"

/**
 * @brief Performs a best-improvement local search using the Warehouse-Swap neighborhood.
 *
 * Explores the full neighborhood N(x) = { (j1, j_close) | j1 is an open warehouse index,
 * j_close ∈ warehouses \ warehouse_assigned }, attempting to close warehouse j1 and open
 * j_close, reassigning all stores of j1 first to j_close and then to other open warehouses
 * if j_close lacks capacity.
 *
 * A move (j1, j_close) is feasible if and only if:
 *   - Every store previously assigned to j1 can be reassigned to j_close or another
 *     open warehouse with sufficient residual capacity.
 *   - All compatibility constraints are satisfied after reassignment.
 *   - The resulting delta (fixed cost difference + supply cost difference) is strictly
 *     negative (improvement).
 *
 * The best feasible move across the entire neighborhood is applied per iteration.
 * The search terminates when no improving move exists in N(x).
 *
 * @param current_solution  The starting solution. The original is not modified.
 * @return A new locally optimal SolutionMSCFLPCI.
 */
SolutionMSCFLPCI* LocalSearchSwapW::solve(const SolutionMSCFLPCI* current_solution) const {
  if (current_solution == nullptr) throw std::invalid_argument("Invalid solution parameter. LocalSearchSwapW::solve");
  const InstanceMSCFLPCI*                instance           = current_solution->getInstanceData();
  if (instance == nullptr) throw std::invalid_argument("Invalid solution parameter. LocalSearchSwapW::solve");
  const short                            num_warehouses     = instance->getNumWarehouses();
  const std::vector<short>&              good               = instance->getGood();
  const std::vector<short>&              capacity           = instance->getCapacity();
  const std::vector<short>&              fixed_cost         = instance->getFixedCost();
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

    float best_delta   = 0.0f;
    short best_j1      = -1;
    short best_j_close = -1;

    for (short j1 = 0; j1 < num_assigned; ++j1) {
      const short j_open = warehouse_assigned[j1];

      for (short j_close = 0; j_close < num_warehouses; ++j_close) {
        if (std::find(warehouse_assigned.begin(), warehouse_assigned.end(), j_close) != warehouse_assigned.end()) continue;

        bool               feasible                  = true;
        short              residual_capacity_j_close = capacity[j_close];
        std::vector<short> tmp_residual_capacity = residual_capacity;
        std::vector<short> assingment_j_close;
        float              delta_supply              = 0.0f;

        for (short i : assignment[j1]) {
          const short amount = static_cast<short>(std::round(good_supplied[i][j1] * good[i]));
          bool assigned = false;

          // 1. try j_close first
          if (residual_capacity_j_close >= amount) {
            if (AlgorithmTools::verifyCompatibility(incompatible_pairs, i, assingment_j_close)) {
              residual_capacity_j_close -= amount;
              assingment_j_close.push_back(i);
              delta_supply              += (supply_cost[i][j_close] - supply_cost[i][j_open]) * amount;
              assigned                   = true;
            }
          } 
          // 2. try others j_open
          else {
            for (short j2 = 0; j2 < num_assigned; ++j2) {
              if (j2 == j1) continue;
              if (tmp_residual_capacity[j2] >= amount) {
                if (AlgorithmTools::verifyCompatibility(incompatible_pairs, i, assignment[j2])) {
                  tmp_residual_capacity[j2] -= amount;
                  delta_supply              += (supply_cost[i][warehouse_assigned[j2]] - supply_cost[i][j_open]) * amount;
                  assigned                   = true;
                  break;
                }
              }
            }
          }
          if (!assigned) { 
            feasible = false; 
            break; 
          }
        }

        if (feasible) {
          const float delta = static_cast<float>(fixed_cost[j_close] - fixed_cost[j_open]) + delta_supply;
          if (delta < best_delta) {
            best_delta = delta;
            best_j1 = j1;
            best_j_close = j_close;
            improved = true;
          }
        }
      }
    }

    if (improved) {
      total_delta           += best_delta;
      const short j_open     = warehouse_assigned[best_j1];
      const short j_close    = best_j_close;
      short j_close_residual = capacity[j_close];      
      
      std::vector<short> i_to_move = assignment[best_j1];
      assignment[best_j1].clear();

      for (short i : i_to_move) {
        const short amount = static_cast<short>(std::round(good_supplied[i][best_j1] * good[i]));
        const float fraction = good_supplied[i][best_j1];
        good_supplied[i][best_j1] = 0.0f;

        bool reassigned = false;
        if (j_close_residual >= amount) {
          if (AlgorithmTools::verifyCompatibility(incompatible_pairs, i, assignment[best_j1])) {
            j_close_residual -= amount;
            assignment[best_j1].push_back(i);
            good_supplied[i][best_j1] = fraction;
            reassigned = true;
          }
        } 
        
        if (!reassigned) {
          for (short j2 = 0; j2 < num_assigned; ++j2) {
            if (j2 == best_j1) continue;
            if (residual_capacity[j2] >= amount) {
              if (AlgorithmTools::verifyCompatibility(incompatible_pairs, i, assignment[j2])) {
                residual_capacity[j2] -= amount;
                if (good_supplied[i][j2] < 1e-6f) {
                  assignment[j2].push_back(i);
                }
                good_supplied[i][j2] += fraction;
                reassigned = true;
                break;
              }
            }
          }
        }
      }
      warehouse_assigned[best_j1] = j_close;
      residual_capacity[best_j1]  = j_close_residual;
    }
  }

  SolutionMSCFLPCI* solution = new SolutionMSCFLPCI(
    instance, warehouse_assigned, assignment, good_supplied, 
    residual_capacity, residual_good
  );
  float epsilon = static_cast<float>(current_solution->getObjectiveValue() + total_delta - solution->getObjectiveValue());
  // if (epsilon > 1) throw std::runtime_error("There is no coincidence between deltas. LocalSearchSwapW::solve");  
  return solution;
}
