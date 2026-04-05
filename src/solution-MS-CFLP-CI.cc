#include "../include/solution-MS-CFLP-CI.h"
#include "../include/instance-MS-CFLP-CI.h"

SolutionMSCFLPCI::SolutionMSCFLPCI(
  const InstanceMSCFLPCI* instance,
  const std::vector<short>& warehouse_assigned,
  const std::vector<std::vector<short>>& assignment,
  const std::vector<std::vector<float>>& good_supplied,
  const std::vector<short>& residual_capacity,
  const std::vector<short>& residual_good
) : 
  instance_data_(instance),
  warehouse_assigned_(warehouse_assigned),
  assignment_(assignment),
  good_supplied_(good_supplied),
  residual_capacity_(residual_capacity),
  residual_good_(residual_good),
  time_(0.0) {
    float f = fixedCostSumFunction();
    float s = supplyCostSumFunction();
    float o = objectiveFunction();
    fixed_costs_sum_ = f;
    supply_costs_sum_ = s;
    objective_value_ = o;
  }

void SolutionMSCFLPCI::setTime(double time) {
  time_ = time;
}

float SolutionMSCFLPCI::fixedCostSumFunction() const {
  const std::vector<short>& fixed_cost = instance_data_->getFixedCost();

  float fixed_cost_sum = 0.0;
  for (short w : warehouse_assigned_) fixed_cost_sum += fixed_cost[w];
  return fixed_cost_sum;
}

float SolutionMSCFLPCI::supplyCostSumFunction() const {
  const short num_stores = instance_data_->getNumStores();
  const short num_assigned = getNumWarehousesAssigned();
  const std::vector<std::vector<short>>& supply_cost = instance_data_->getSupplyCost();
  const std::vector<short>& good = instance_data_->getGood();

  float supply_cost_sum = 0.0;
  for (short j = 0; j < num_assigned; ++j) {
    for (short store : assignment_[j]) {
      short i = store - 1;
      if (good_supplied_[i][j] > 0.0) {
        supply_cost_sum += supply_cost[i][warehouse_assigned_[j]] * good[i] * good_supplied_[i][j];
      }
    }
  }
  return supply_cost_sum;
}

float SolutionMSCFLPCI::objectiveFunction() const {
  return fixedCostSumFunction() + supplyCostSumFunction();
}

// Getters
const InstanceMSCFLPCI* SolutionMSCFLPCI::getInstanceData() const {
  return instance_data_;
}

const short SolutionMSCFLPCI::getNumWarehousesAssigned() const {
  return static_cast<short>(warehouse_assigned_.size());
}

const std::vector<short>& SolutionMSCFLPCI::getWarehouseAssigned() const {
  return warehouse_assigned_;
}

const std::vector<std::vector<short>>& SolutionMSCFLPCI::getAssignment() const {
  return assignment_;
}

const std::vector<std::vector<float>>& SolutionMSCFLPCI::getGoodSupplied() const {
  return good_supplied_;
}

const std::vector<short>& SolutionMSCFLPCI::getResidualCapacity() const {
  return residual_capacity_;
}

const std::vector<short>& SolutionMSCFLPCI::getResidualGood() const {
  return residual_good_;
}

const double& SolutionMSCFLPCI::getTime() const {
  return time_;
}

const float& SolutionMSCFLPCI::getFixedCostsSum() const {
  return fixed_costs_sum_;
}

const float& SolutionMSCFLPCI::getSupplyCostsSum() const {
  return supply_costs_sum_;
}

const float& SolutionMSCFLPCI::getObjectiveValue() const {
  return objective_value_;
}