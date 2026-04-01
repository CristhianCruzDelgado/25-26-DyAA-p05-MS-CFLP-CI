#include "../include/algorithm-greedy-MS-CFLP-CI.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"

AlgorithmGreedyMS_CFLP_CI::AlgorithmGreedyMS_CFLP_CI(
  const short& slack
) : 
  slack_(slack) {}

Solution* AlgorithmGreedyMS_CFLP_CI::solve(const Instance* instance) const {
  const InstanceMS_CFLP_CI* instance_MS_CFLP_CI = dynamic_cast<const InstanceMS_CFLP_CI*>(instance);
  if (instance_MS_CFLP_CI == nullptr) throw std::invalid_argument("Invalid instance type");

  // Phase 1: Warehouse selection
  const short& num_warehouses = instance_MS_CFLP_CI->getNumWarehouses();
  const short& num_stores = instance_MS_CFLP_CI->getNumStores();
  const std::vector<short>& fixed_cost = instance_MS_CFLP_CI->getFixedCost();
  const std::vector<short>& good = instance_MS_CFLP_CI->getGood();
  const std::vector<short>& capacity = instance_MS_CFLP_CI->getCapacity();
  // 1
  std::vector<short> sorted_fixed_cost_indices = sortFixedCostAscending(fixed_cost, num_warehouses); 
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
    // if (capacity_sum < goods_sum)
    // 7
    warehouse_assigned.push_back(sorted_fixed_cost_indices[i]);
    // 8
    capacity_sum += capacity[sorted_fixed_cost_indices[i]];
  }
  // 9
  std::vector<short> extra_warehouses;
  for (short j = i; j < i + slack_ && j < num_warehouses; ++j)
    extra_warehouses.push_back(sorted_fixed_cost_indices[j]);
  // 10
  for (short e : extra_warehouses) warehouse_assigned.push_back(e);




  // Phase 2: Store assignment
  const short num_assigned = static_cast<short>(warehouse_assigned.size());
  const std::vector<std::vector<short>>& supply_cost = instance_MS_CFLP_CI->getSupplyCost();
  const std::vector<ShortPair>& incompatible_pairs = instance_MS_CFLP_CI->getIncompatiblePairs();
  // 11
  std::vector<short> residual_good = good;
  std::vector<short> residual_capacity(num_assigned);
  for (short i = 0; i < num_assigned; ++i)
    residual_capacity[i] = capacity[warehouse_assigned[i]];
  // 12
  std::vector<std::vector<short>> assignment(num_assigned);
  // 13
  std::vector<std::vector<float>> good_supplied(num_stores, std::vector<float>(num_assigned, 0.0));
  // 14
  for(short i = 0; i < num_stores; ++i) {
    // 15
    std::vector<short> sorted_store_supply_cost_indices = sortSupplyCostAscending(warehouse_assigned, num_assigned, supply_cost[i]);
    // 16
    short k = 0;
    while (residual_good[i] > 0 && k < num_assigned) {
      // 17
      short j = sorted_store_supply_cost_indices[k];
      // 18
      short store = i + 1;
      if (verifyCompatibility(incompatible_pairs, store, assignment[j])) {
        // 19
        short supply_amount = std::min(residual_good[i], residual_capacity[j]);
        if (supply_amount > 0) { 
          // 20
          good_supplied[i][j] += supply_amount / static_cast<float>(good[i]);
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
  Solution* solution = new SolutionMS_CFLP_CI(
    instance_MS_CFLP_CI, 
    std::move(warehouse_assigned),
    std::move(assignment), 
    std::move(good_supplied), 
    std::move(residual_capacity), 
    std::move(residual_good));
  return solution;
}

// Position i of the returned vector contains the index of the warehouse with the i-th lowest fixed cost
std::vector<short> AlgorithmGreedyMS_CFLP_CI::sortFixedCostAscending(const std::vector<short>& v, const short& size) const {
  std::vector<short> indices(size);
  for (short i = 0; i < size; ++i)
    indices[i] = i;
  std::sort(indices.begin(), indices.end(), [&](short a, short b) {
    return v[a] < v[b];
  });
  return indices;
}

// Position i of the returned vector contains the index of the warehouse with the i-th lowest supply cost for store i
std::vector<short> AlgorithmGreedyMS_CFLP_CI::sortSupplyCostAscending(std::vector<short> warehouse_assigned, const short& size, const std::vector<short>& v) const {
  std::vector<short> indices(size);
  for (short i = 0; i < size; ++i)
    indices[i] = i;
  std::sort(indices.begin(), indices.end(), [&](short a, short b) {
    return v[warehouse_assigned[a]] < v[warehouse_assigned[b]];
  });
  return indices;
}

bool AlgorithmGreedyMS_CFLP_CI::verifyCompatibility(const std::vector<ShortPair>& incompatible_pairs, const short& store, const std::vector<short>& assingments) const {
  if (assingments.empty()) return true;
  for (const ShortPair& pair : incompatible_pairs) 
    if (pair.first == store) 
      for (short assigned_store : assingments) 
        if (assigned_store == pair.second) 
          return false;
  return true;
}