#include "../include/algorithm-greedy-MS-CFLP-CI.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"

AlgorithmGreedyMSCFLPCI::AlgorithmGreedyMSCFLPCI(
  const short& slack
) : 
  slack_(slack) {}

Solution* AlgorithmGreedyMSCFLPCI::solve(const Instance* instance) const {
  const InstanceMSCFLPCI* instance_MSCFLPCI = dynamic_cast<const InstanceMSCFLPCI*>(instance);
  if (instance_MSCFLPCI == nullptr) throw std::invalid_argument("Invalid instance type. AlgorithmGreedyMSCFLPCI::solve");

  // Phase 1: Warehouse selection
  const short& num_warehouses = instance_MSCFLPCI->getNumWarehouses();
  const std::vector<short>& fixed_cost = instance_MSCFLPCI->getFixedCost();
  const std::vector<short>& good = instance_MSCFLPCI->getGood();
  const std::vector<short>& capacity = instance_MSCFLPCI->getCapacity();
  // 1
  std::vector<short> sorted_fixed_cost_indices = AlgorithmTools::sortFixedCostAscending(fixed_cost, num_warehouses); 
  // 2
  short goods_sum = 0;
  for (short g : good) goods_sum += g;
  // 3
  short capacity_sum = 0;
  // 4
  std::vector<short> warehouse_assigned;
  // 5
  short i = 0;
  for (; i < num_warehouses && capacity_sum < goods_sum; ++i) {
    // 6
    // if (capacity_sum < goods_sum)       ^^^
    // 7
    short w = sorted_fixed_cost_indices[i];
    warehouse_assigned.push_back(w);
    // 8
    capacity_sum += capacity[w];
  }
  // 9 & 10
  // std::vector<short> extra_warehouses;
  for (short j = i; j < i + slack_ && j < num_warehouses; ++j)
    // extra_warehouses.push_back(sorted_fixed_cost_indices[j]);
    warehouse_assigned.push_back(sorted_fixed_cost_indices[j]);
  // 10
  // for (short e : extra_warehouses) warehouse_assigned.push_back(e);

  // Phase 2: Store assignment
  const short& num_stores = instance_MSCFLPCI->getNumStores();
  const short num_assigned = static_cast<short>(warehouse_assigned.size());
  const std::vector<std::vector<short>>& supply_cost = instance_MSCFLPCI->getSupplyCost();
  const std::vector<ShortPair>& incompatible_pairs = instance_MSCFLPCI->getIncompatiblePairs();
  // 11
  std::vector<short> residual_good = good;
  std::vector<short> residual_capacity(num_assigned);
  for (short i = 0; i < num_assigned; ++i)
    residual_capacity[i] = capacity[warehouse_assigned[i]];
  // 12
  std::vector<std::vector<short>> assignment(num_assigned);
  // 13
  std::vector<std::vector<float>> good_supplied(num_stores, std::vector<float>(num_assigned, 0.0f));
  // 14
  std::vector<short> sorted_supply_cost_indices;
  sorted_supply_cost_indices.reserve(num_assigned);
  for(short i = 0; i < num_stores; ++i) {
    // 15
    sorted_supply_cost_indices = AlgorithmTools::sortSupplyCostAscending(warehouse_assigned, num_assigned, supply_cost[i]);
    // 18
    short store = i + 1;
    // 16
    float good_i = static_cast<float>(good[i]);
    short k = 0;
    while (residual_good[i] > 0 && k < num_assigned) {
      // 17
      short j = sorted_supply_cost_indices[k];
      if (AlgorithmTools::verifyCompatibility(incompatible_pairs, store, assignment[j])) {
        // 19
        short supply_amount = std::min(residual_good[i], residual_capacity[j]);
        if (supply_amount > 0) { 
          // 20
          good_supplied[i][j] += supply_amount / good_i;
          // 21
          residual_capacity[j] -= supply_amount;
          // 22
          residual_good[i] -= supply_amount;
          // 23 
          assignment[j].push_back(store);
        }
      }
      ++k;
    }
  }

  // 24
  // float fixed_cost_sum = calculateFixedCostSum();
  // 25
  // float supply_cost_t_sum = calculateSupplyCostSum();
  // 26
  // float total_cost = calculateTotalCost();
  // 27
  Solution* solution = new SolutionMSCFLPCI(
    instance_MSCFLPCI, 
    std::move(warehouse_assigned),
    std::move(assignment), 
    std::move(good_supplied), 
    std::move(residual_capacity), 
    std::move(residual_good)
  );
  return solution;
}
