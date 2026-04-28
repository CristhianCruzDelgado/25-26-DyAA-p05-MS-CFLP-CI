#include "../include/instance-MS-CFLP-CI.h"
#include "../include/algorithm.h"

InstanceMSCFLPCI::InstanceMSCFLPCI(
  const std::vector<short>& capacity, 
  const std::vector<short>& fixed_cost, 
  const std::vector<short>& good, 
  const std::vector<std::vector<short>>& supply_cost, 
  const std::vector<ShortPair>& incompatible_pairs
) : 
  capacity_(capacity), 
  fixed_cost_(fixed_cost), 
  good_(good), 
  supply_cost_(supply_cost), 
  incompatible_pairs_(incompatible_pairs),
  algorithm_(nullptr) {}

void InstanceMSCFLPCI::setAlgorithm(const Algorithm* algorithm) {
  if (algorithm == nullptr) throw std::runtime_error("Algorithm cannot be null. InstanceMSCFLPCI::setAlgorithm");
  algorithm_ = algorithm;
}

Solution* InstanceMSCFLPCI::solve() const {
  if (algorithm_ == nullptr) throw std::runtime_error("Algorithm not set. InstanceMSCFLPCI::solve");
  return algorithm_->solve(this);
}

// Getters
const short InstanceMSCFLPCI::getNumWarehouses() const {
  return static_cast<short>(capacity_.size());
}

const short InstanceMSCFLPCI::getNumStores() const {
  return static_cast<short>(good_.size());
}  

const std::vector<short>& InstanceMSCFLPCI::getCapacity() const {
  return capacity_;
}

const std::vector<short>& InstanceMSCFLPCI::getFixedCost() const {
  return fixed_cost_;
}

const std::vector<short>& InstanceMSCFLPCI::getGood() const {
  return good_;
}

const std::vector<std::vector<short>>& InstanceMSCFLPCI::getSupplyCost() const {
  return supply_cost_;
}

const short InstanceMSCFLPCI::getNumIncompatibilities() const {
  return static_cast<short>(incompatible_pairs_.size()) / 2;
}

const std::vector<ShortPair>& InstanceMSCFLPCI::getIncompatiblePairs() const {
  return incompatible_pairs_;
}
