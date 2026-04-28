#include "../include/shaking-shift.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"

SolutionMSCFLPCI* ShakingShift::shake(const SolutionMSCFLPCI* current_solution, const short& max_k) const {
  if (current_solution == nullptr) throw std::invalid_argument("Invalid solution parameter. ShakingShift::solve");
  const InstanceMSCFLPCI*                instance           = current_solution->getInstanceData();
  if (instance == nullptr) throw std::invalid_argument("Invalid solution parameter. ShakingShift::solve");
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

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<short> dist(0, num_stores - 1);

  float total_delta = 0.0;
  short k = 1;
  while (k <= max_k) {

    float feasible_delta  = 0.0f;
    short feasible_i      = -1;
    short feasible_j1     = -1;
    short feasible_j2     = -1;
    float feasible_amount = 0.0;

    for (short _ = 0; _ < num_stores; ++_) {
      const short i = dist(gen);

      for (short j1 = 0; j1 < num_assigned; ++j1) {
        if (good_supplied[i][j1] <= 1e-6f) continue; 
        const short amount = static_cast<short>(std::round(good_supplied[i][j1] * good[i]));
        if (amount == 0) continue;

        for (short j2 = 0; j2 < num_assigned; ++j2) {
          if (j1 == j2) continue;
          if (residual_capacity[j2] == 0) continue;
          if (!AlgorithmTools::verifyCompatibility(incompatible_pairs, i, assignment[j2])) continue;
          feasible_amount = std::min(amount, residual_capacity[j2]);
          if (feasible_amount == 0) continue;
          feasible_delta = (supply_cost[i][warehouse_assigned[j2]] - supply_cost[i][warehouse_assigned[j1]]) * feasible_amount;
          feasible_i     = i;
          feasible_j1    = j1;
          feasible_j2    = j2;
          break;
        }
      }
      if (feasible_i != -1) break;
    }

    if (feasible_i == -1) break;

    total_delta += feasible_delta;

    const short full_amount = static_cast<short>(std::round(good_supplied[feasible_i][feasible_j1] * good[feasible_i]));
    const float fraction = feasible_amount / (float)good[feasible_i];

    if (std::abs(fraction - full_amount) < 1e-6f) {
      // Full shift: remove store from j1
      assignment[feasible_j1].erase(std::find(assignment[feasible_j1].begin(), assignment[feasible_j1].end(), feasible_i));
      good_supplied[feasible_i][feasible_j1] = 0.0f;
    } else {
      // Partial shift: reduce fraction in j1
      good_supplied[feasible_i][feasible_j1] -= fraction;
    }

    if (good_supplied[feasible_i][feasible_j2] < 1e-6f) {
      assignment[feasible_j2].push_back(feasible_i);
    }

    good_supplied[feasible_i][feasible_j2]  += fraction;
    residual_capacity[feasible_j1] += feasible_amount;
    residual_capacity[feasible_j2] -= feasible_amount;

    ++k;
  }

  SolutionMSCFLPCI* solution = new SolutionMSCFLPCI(
    instance, warehouse_assigned, assignment, good_supplied, 
    residual_capacity, residual_good
  );
  float epsilon = static_cast<float>(current_solution->getObjectiveValue() + total_delta - solution->getObjectiveValue());
  // if (epsilon > 1) throw std::runtime_error("There is no coincidence between deltas. ShakingShift::solve");
  return solution;
}
