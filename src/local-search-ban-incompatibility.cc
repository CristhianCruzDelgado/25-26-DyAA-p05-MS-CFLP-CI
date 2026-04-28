#include "../include/local-search-ban-incompatibility.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"

/**
 * @brief Performs a best-improvement local search using the Incompatibility-Ban neighborhood.
 *
 * Each iteration identifies the open warehouse with the highest total supply cost
 * and within it the store that causes the most active incompatibility conflicts. 
 * It then evaluates all alternative open warehouses and select the one that yields 
 * the greatest improvement in supply cost for relocating.
 *
 * A move (block_i, worst_j, j2) is feasible if and only if:
 *   - j2 has enough residual capacity to absorb the amount supplied to block_i.
 *   - block_i is compatible with all stores currently assigned to j2.
 *   - The resulting delta in supply cost is strictly negative (improvement).
 *
 * Unlike a pure Shift, the candidate store is selected by incompatibility pressure rather
 * than exhaustive enumeration, making this neighborhood particularly effective in instances
 * with dense incompatibility constraints.
 *
 * The best feasible destination across all open warehouses is applied per iteration.
 * The search terminates when no improving move exists.
 *
 * @param current_solution  The starting solution. The original is not modified.
 * @return A new locally optimal SolutionMSCFLPCI.
 */
SolutionMSCFLPCI* LocalSearchBanIncompatibility::solve(const SolutionMSCFLPCI* current_solution) const {
  if (current_solution == nullptr) throw std::invalid_argument("Invalid solution parameter. LocalSearchBanIncompatibility::solve");
  const InstanceMSCFLPCI*                instance           = current_solution->getInstanceData();
  if (instance == nullptr) throw std::invalid_argument("Invalid solution parameter. LocalSearchBanIncompatibility::solve");
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

    short worst_j           = -1;
    float worst_supply_cost = -1.0f;

    for (short j1 = 0; j1 < num_assigned; ++j1) {
      float total = 0.0f;
      for (short i : assignment[j1]) {
        const short amount = static_cast<short>(std::round(good_supplied[i][j1] * good[i]));
        total += supply_cost[i][warehouse_assigned[j1]] * amount;
      }
      if (total > worst_supply_cost) {
        worst_supply_cost = total;
        worst_j           = j1;
      }
    }

    if (worst_j == -1) continue;

    short block_i               = -1;
    short max_incompatibilities = -1;

    for (short i : assignment[worst_j]) {
      short incompatibilities = 0;

      for (const ShortPair& pair : incompatible_pairs)
        if (pair.first == i)
          for (short other : assignment[worst_j])
            if (other == pair.second) ++incompatibilities;

      if (incompatibilities > max_incompatibilities) {
        block_i               = i;
        max_incompatibilities = incompatibilities;
      }
    }

    if (block_i == -1) continue;

    // worst_j found and block_i found

    const short amount     = static_cast<short>(std::round((good_supplied[block_i][worst_j] * good[block_i])));
    float       best_delta = 0.0f;
    short       best_j     = -1;

    for (short j2 = 0; j2 < num_assigned; ++j2) {
      if (j2 == worst_j) continue;
      if (residual_capacity[j2] < amount) continue;
      if (!AlgorithmTools::verifyCompatibility(incompatible_pairs, block_i, assignment[j2])) continue;

      const float delta = (supply_cost[block_i][warehouse_assigned[j2]] - supply_cost[block_i][warehouse_assigned[worst_j]]) * amount;
      if (delta < best_delta) {
        improved   = true;
        best_delta = delta;
        best_j     = j2;
      }
    }

    if (improved) {
      total_delta                    += best_delta;
      auto it = std::find(assignment[worst_j].begin(), assignment[worst_j].end(), block_i);
      assignment[worst_j].erase(it);
      if (good_supplied[block_i][best_j] == 0.0f) {
        assignment[best_j].push_back(block_i);
      }
      good_supplied[block_i][best_j] += good_supplied[block_i][worst_j];
      good_supplied[block_i][worst_j] = 0.0f;
      residual_capacity[worst_j]     += amount;
      residual_capacity[best_j]      -= amount;
    }
  }

  // 1. Identificar almacenes que realmente tienen tiendas asignadas
  std::vector<short>              new_warehouse_assigned;
  std::vector<std::vector<short>> new_assignment;
  std::vector<short>              new_residual_capacity;
  std::vector<short>              new_residual_good;

  // Mapeo para actualizar la matriz good_supplied: [tienda][nuevo_indice_columna]
  std::vector<std::vector<float>> new_good_supplied(instance->getNumStores(), std::vector<float>());

  for (short kj = 0; kj < warehouse_assigned.size(); ++kj) {
      // Si el almacén tiene al menos una tienda y suministra algo de mercancía
      if (!assignment[kj].empty()) {
          
          // Mantener este almacén
          new_warehouse_assigned.push_back(warehouse_assigned[kj]);
          new_assignment.push_back(assignment[kj]);
          new_residual_capacity.push_back(residual_capacity[kj]);
          new_residual_good.push_back(residual_good[kj]);

          // Copiar la columna de suministros de este almacén
          for (short i = 0; i < instance->getNumStores(); ++i) {
              new_good_supplied[i].push_back(good_supplied[i][kj]);
          }
      }
  }

  SolutionMSCFLPCI* solution = new SolutionMSCFLPCI(
    instance, warehouse_assigned, assignment, good_supplied, 
    residual_capacity, residual_good
  );
  float epsilon = static_cast<float>(current_solution->getObjectiveValue() + total_delta - solution->getObjectiveValue());
  // if (epsilon > 1) throw std::runtime_error("There is no coincidence between deltas. LocalSearchBanIncompatibility::solve");  
  return solution;
}
