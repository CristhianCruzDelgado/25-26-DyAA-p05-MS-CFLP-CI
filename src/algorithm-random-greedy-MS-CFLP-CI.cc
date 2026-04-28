#include "../include/algorithm-random-greedy-MS-CFLP-CI.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"
#include "../include/local-search.h"

AlgorithmRandomGreedyMSCFLPCI::AlgorithmRandomGreedyMSCFLPCI(
  const short& slack,
  const short& rcl_size
) :
  slack_(slack),
  rcl_size_(rcl_size) {}

Solution* AlgorithmRandomGreedyMSCFLPCI::solve(const Instance* instance) const {
  if (instance == nullptr) throw std::invalid_argument("Invalid instance parameter. AlgorithmRandomGreedyMSCFLPCI::solve");
  const InstanceMSCFLPCI* instance_MSCFLPCI = dynamic_cast<const InstanceMSCFLPCI*>(instance);
  if (instance_MSCFLPCI == nullptr) throw std::invalid_argument("Invalid instance type. AlgorithmRandomGreedyMSCFLPCI::solve");
  std::random_device rd;
  std::mt19937 gen(rd());

  // Phase 1: Warehouse selection
  const short& num_warehouses = instance_MSCFLPCI->getNumWarehouses();
  const std::vector<short>& fixed_cost = instance_MSCFLPCI->getFixedCost();
  const std::vector<short>& good = instance_MSCFLPCI->getGood();
  const std::vector<short>& capacity = instance_MSCFLPCI->getCapacity();

  // 1 - 4
  std::vector<short> sorted_fixed_cost_indices = AlgorithmTools::sortFixedCostAscending(fixed_cost, num_warehouses); 
  short goods_sum = 0;
  for (short g : good) goods_sum += g;
  short capacity_sum = 0;
  std::vector<short> warehouse_assigned;

  std::vector<short> rcl;
  rcl.reserve(slack_);

  // 5 - 8
  for (short it1 = 0; it1 < num_warehouses && capacity_sum < goods_sum; ++it1) {
    rcl.clear();
    short rcl_size = std::min(rcl_size_, static_cast<short>(sorted_fixed_cost_indices.size()));
    for (short it2 = 0; it2 < rcl_size; ++it2) {
      short j = sorted_fixed_cost_indices[it2];
      rcl.push_back(j);
    }
    std::uniform_int_distribution<short> dist(0, rcl.size() - 1);
    short pos = dist(gen);
    short j = rcl[pos];
    warehouse_assigned.push_back(j);
    capacity_sum += capacity[j];
    sorted_fixed_cost_indices.erase(sorted_fixed_cost_indices.begin() + pos);
  }

  // 9 - 10
  for (short it3 = 0; it3 < slack_; ++it3) {
    short j = sorted_fixed_cost_indices[it3];
    warehouse_assigned.push_back(j);
  }

  // Phase 2: Store assignment
  const short& num_stores = instance_MSCFLPCI->getNumStores();
  const short num_assigned = static_cast<short>(warehouse_assigned.size());
  const std::vector<std::vector<short>>& supply_cost = instance_MSCFLPCI->getSupplyCost();
  const std::vector<ShortPair>& incompatible_pairs = instance_MSCFLPCI->getIncompatiblePairs();

  // 11 -13
  std::vector<short> residual_good = good;
  std::vector<short> residual_capacity(num_assigned);
  for (short j = 0; j < num_assigned; ++j)
    residual_capacity[j] = capacity[warehouse_assigned[j]];
  std::vector<std::vector<short>> assignment(num_assigned);
  std::vector<std::vector<float>> good_supplied(num_stores, std::vector<float>(num_assigned, 0.0f));

  // 14 - 23
  std::vector<short> sorted_supply_cost_indices;
  sorted_supply_cost_indices.reserve(num_assigned);
  for(short i = 0; i < num_stores; ++i) {
    sorted_supply_cost_indices = AlgorithmTools::sortSupplyCostAscending(warehouse_assigned, num_assigned, supply_cost[i]);
    float good_i = static_cast<float>(good[i]);
    while (residual_good[i] > 0 && !sorted_supply_cost_indices.empty()) {
      rcl.clear();
      short rcl_size = std::min(rcl_size_, static_cast<short>(sorted_supply_cost_indices.size()));
      for (short it4 = 0; it4 < rcl_size; ++it4) {
        short j = sorted_supply_cost_indices[it4];
        rcl.push_back(j);
      }
      std::uniform_int_distribution<short> dist(0, rcl.size() - 1);
      short pos = dist(gen);
      short j = rcl[pos];
      if (AlgorithmTools::verifyCompatibility(incompatible_pairs, i, assignment[j])) {
        short supply_amount = std::min(residual_good[i], residual_capacity[j]);
        if (supply_amount > 0) { 
          good_supplied[i][j] += supply_amount / good_i;
          residual_capacity[j] -= supply_amount;
          residual_good[i] -= supply_amount;
          assignment[j].push_back(i);
        } 
        if (residual_capacity[j] == 0) {
          sorted_supply_cost_indices.erase(sorted_supply_cost_indices.begin() + pos);
        }
      } else {
        sorted_supply_cost_indices.erase(sorted_supply_cost_indices.begin() + pos);
      }
    }
  }

  SolutionMSCFLPCI* solution = new SolutionMSCFLPCI(
    instance_MSCFLPCI, warehouse_assigned, assignment, good_supplied, 
    residual_capacity, residual_good
  );
  return solution;
}
