#include "../include/algorithm-GRASP-MS-CFLP-CI.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"
#include "../include/local-search.h"

AlgorithmGraspMSCFLPCI::AlgorithmGraspMSCFLPCI(
  const short& grasp_iterations,
  const short& rcl_size,
  const short& slack,
  const LocalSearch* local_search
) :
  grasp_iterations_(grasp_iterations),
  rcl_size_(rcl_size),
  slack_(slack),
  objective_value_construct_solution_(0.0),
  local_search_(local_search) {}

Solution* AlgorithmGraspMSCFLPCI::solve(const Instance* instance) const {
  const InstanceMSCFLPCI* instance_MSCFLPCI = dynamic_cast<const InstanceMSCFLPCI*>(instance);
  if (instance_MSCFLPCI == nullptr) throw std::invalid_argument("Invalid instance type. AlgorithmGRASPMSCFLPCI::solve");
  if (local_search_ == nullptr) throw std::runtime_error("LocalSearch not set. AlgorithmGRASPMSCFLPCI::solve");

  SolutionMSCFLPCI* best_solution = nullptr;

  for (short i = 0; i < grasp_iterations_; ++i) {
    SolutionMSCFLPCI* current_solution = construct(instance_MSCFLPCI);
    SolutionMSCFLPCI* improved_solution = local_search_->solve(current_solution);
    delete current_solution;
    if (best_solution == nullptr || improved_solution->getObjectiveValue() < best_solution->getObjectiveValue()) {
      delete best_solution;
      best_solution = improved_solution;
    } else {
      delete improved_solution;
    }
  }

  Solution* solution = best_solution;
  return solution;
}

SolutionMSCFLPCI* AlgorithmGraspMSCFLPCI::construct(const InstanceMSCFLPCI* instance_MSCFLPCI) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<short> rcl;

  // Phase 1: Warehouse selection
  const short& num_warehouses = instance_MSCFLPCI->getNumWarehouses();
  const std::vector<short>& fixed_cost = instance_MSCFLPCI->getFixedCost();
  const std::vector<short>& good = instance_MSCFLPCI->getGood();
  const std::vector<short>& capacity = instance_MSCFLPCI->getCapacity();
  std::vector<short> sorted_fixed_cost_indices = AlgorithmTools::sortFixedCostAscending(fixed_cost, num_warehouses); 
  short goods_sum = 0;
  for (short g : good) goods_sum += g;
  short capacity_sum = 0;
  std::vector<short> warehouse_assigned;
  short i = 0;
  for (; i < num_warehouses && capacity_sum < goods_sum; ++i) {
    for (short j = 0; j < rcl_size_ && j < sorted_fixed_cost_indices.size(); ++j)
      rcl.push_back(sorted_fixed_cost_indices[j]);
    if (rcl.empty()) break;
    std::uniform_int_distribution<short> dist(0, rcl.size() - 1);
    short element = rcl[dist(gen)];
    warehouse_assigned.push_back(element);
    capacity_sum += capacity[element];
    auto it = std::find(sorted_fixed_cost_indices.begin(), sorted_fixed_cost_indices.end(), element);
    sorted_fixed_cost_indices.erase(it);
    rcl.clear();
  }
  for (short j = i; j < i + slack_ && j < num_warehouses; ++j) {
    for (short k = 0; k < rcl_size_ && k < sorted_fixed_cost_indices.size(); ++k)
      rcl.push_back(sorted_fixed_cost_indices[k]);
    if (rcl.empty()) break;
    std::uniform_int_distribution<short> dist(0, rcl.size() - 1);
    short element = rcl[dist(gen)];
    warehouse_assigned.push_back(element);
    auto it = std::find(sorted_fixed_cost_indices.begin(), sorted_fixed_cost_indices.end(), element);
    sorted_fixed_cost_indices.erase(it);
    rcl.clear();
  }




  // Phase 2: Store assignment
  const short& num_stores = instance_MSCFLPCI->getNumStores();
  const short num_assigned = static_cast<short>(warehouse_assigned.size());
  const std::vector<std::vector<short>>& supply_cost = instance_MSCFLPCI->getSupplyCost();
  const std::vector<ShortPair>& incompatible_pairs = instance_MSCFLPCI->getIncompatiblePairs();
  std::vector<short> residual_good = good;
  std::vector<short> residual_capacity(num_assigned);
  for (short i = 0; i < num_assigned; ++i)
    residual_capacity[i] = capacity[warehouse_assigned[i]];
  std::vector<std::vector<short>> assignment(num_assigned);
  std::vector<std::vector<float>> good_supplied(num_stores, std::vector<float>(num_assigned, 0.0));
  for(short i = 0; i < num_stores; ++i) {
    std::vector<short> sorted_supply_cost_indices = AlgorithmTools::sortSupplyCostAscending(warehouse_assigned, num_assigned, supply_cost[i]);
    while (residual_good[i] > 0 && !sorted_supply_cost_indices.empty()) {
      for (short j = 0; j < rcl_size_ && j < sorted_supply_cost_indices.size(); ++j)
        rcl.push_back(sorted_supply_cost_indices[j]);
      if (rcl.empty()) break;
      std::uniform_int_distribution<short> dist(0, rcl.size() - 1);
      short element = rcl[dist(gen)];
      short store = i + 1;
      if (AlgorithmTools::verifyCompatibility(incompatible_pairs, store, assignment[element])) {
        short supply_amount = std::min(residual_good[i], residual_capacity[element]);
        if (supply_amount > 0) { 
          good_supplied[i][element] += supply_amount / static_cast<float>(good[i]);
          residual_capacity[element] -= supply_amount;
          residual_good[i] -= supply_amount;
          assignment[element].push_back(store);
        }
      }
      auto it = std::find(sorted_supply_cost_indices.begin(), sorted_supply_cost_indices.end(), element);
      sorted_supply_cost_indices.erase(it);
      rcl.clear();
    }
  }

  SolutionMSCFLPCI* solution_MSCFLPCI = new SolutionMSCFLPCI(
    instance_MSCFLPCI, 
    std::move(warehouse_assigned),
    std::move(assignment), 
    std::move(good_supplied), 
    std::move(residual_capacity), 
    std::move(residual_good));
  return solution_MSCFLPCI;
}
