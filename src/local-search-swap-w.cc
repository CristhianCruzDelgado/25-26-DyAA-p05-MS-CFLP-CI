#include "../include/local-search-swap-w.h"
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
 * @param current_solution  The starting solution. Its data is copied; the
 *                          original is not modified.
 * @return A new locally optimal SolutionMSCFLPCI. The caller takes ownership.
 */
SolutionMSCFLPCI* LocalSearchSwapW::solve(SolutionMSCFLPCI* current_solution) const {
  const InstanceMSCFLPCI* instance = current_solution->getInstanceData();
  const short num_warehouses = instance->getNumWarehouses();
  const std::vector<short>& good = instance->getGood();
  const std::vector<short>& capacity = instance->getCapacity();
  const std::vector<short>& fixed_cost = instance->getFixedCost();
  const std::vector<std::vector<short>>& supply_cost = instance->getSupplyCost();
  const std::vector<ShortPair>& incompatible_pairs = instance->getIncompatiblePairs();
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
    short best_j1 = -1;
    short best_j_close = -1;
    std::vector<short> best_residual_capacity;
    std::vector<std::vector<short>> best_assignment;
    std::vector<std::vector<float>> best_good_supplied;

    for (short j1 = 0; j1 < num_assigned; ++j1) {
      const short w_open = warehouse_assigned[j1];

      for (short j_close = 0; j_close < num_warehouses; ++j_close) {
        if (std::find(warehouse_assigned.begin(), warehouse_assigned.end(), j_close) != warehouse_assigned.end()) continue;

        std::vector<short> sim_residual_capacity = residual_capacity;
        std::vector<std::vector<short>> sim_assignment = assignment;
        std::vector<std::vector<float>> sim_good_supplied = good_supplied;
        sim_residual_capacity[j1] = 0;

        short j_close_residual = capacity[j_close];
        std::vector<short> j_close_assigned;

        bool feasible = true;
        float delta_supply = 0.0;

        for (short s_idx = 0; s_idx < assignment[j1].size() && feasible; ++s_idx) {
          const short store = assignment[j1][s_idx];
          const short i = store - 1;
          const short amount = static_cast<short>(good_supplied[i][j1] * good[i]);
          bool assigned = false;

          if (j_close_residual >= amount && AlgorithmTools::verifyCompatibility(incompatible_pairs, store, j_close_assigned)) {
            j_close_residual -= amount;
            j_close_assigned.push_back(store);
            delta_supply += (supply_cost[i][j_close] - supply_cost[i][w_open]) * amount;
            sim_good_supplied[i][j1] = 0.0;
            sim_good_supplied[i][j1] = static_cast<float>(amount) / good[i];
            sim_assignment[j1].push_back(store);
            assigned = true;
          }

          if (!assigned) {
            for (short j2 = 0; j2 < num_assigned && !assigned; ++j2) {
              if (j2 == j1) continue;
              const short w_b = warehouse_assigned[j2];
              std::vector<short> j2_without_store;
              for (short x : sim_assignment[j2]) if (x != store) j2_without_store.push_back(x);
              if (sim_residual_capacity[j2] >= amount &&
                  AlgorithmTools::verifyCompatibility(incompatible_pairs, store, j2_without_store)) {
                sim_residual_capacity[j2] -= amount;
                delta_supply += (supply_cost[i][w_b] - supply_cost[i][w_open]) * amount;
                sim_good_supplied[i][j2] += good_supplied[i][j1];
                sim_good_supplied[i][j1]  = 0.0f;
                sim_assignment[j2].push_back(store);
                sim_assignment[j1].erase(std::find(sim_assignment[j1].begin(), sim_assignment[j1].end(), store));
                assigned = true;
              }
            }
          }
          if (!assigned) feasible = false;
        }
        if (!feasible) continue;
        const float delta = (fixed_cost[j_close] - fixed_cost[w_open]) + delta_supply;
        if (delta < best_delta) {
          best_delta = delta;
          best_j1 = j1;
          best_j_close = j_close;
          sim_residual_capacity[j1] = j_close_residual;
          best_residual_capacity = sim_residual_capacity;
          best_assignment = sim_assignment;
          best_good_supplied = sim_good_supplied;
        }
      }
    }
    if (best_j1 != -1) {
      warehouse_assigned[best_j1] = best_j_close;
      residual_capacity = best_residual_capacity;
      assignment = best_assignment;
      good_supplied = best_good_supplied;
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