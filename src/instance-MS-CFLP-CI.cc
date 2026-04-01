#include "../include/instance-MS-CFLP-CI.h"
#include "../include/algorithm.h"

InstanceMS_CFLP_CI::InstanceMS_CFLP_CI(
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

InstanceMS_CFLP_CI::~InstanceMS_CFLP_CI() {
  delete algorithm_;
}

void InstanceMS_CFLP_CI::setAlgorithm(const Algorithm* algorithm) {
  if (algorithm == nullptr) throw std::runtime_error("Algorithm cannot be null. InstanceMS_CFLP_CI::setAlgorithm");
  algorithm_ = algorithm;
}

Solution* InstanceMS_CFLP_CI::solve() {
  if (algorithm_ == nullptr) throw std::runtime_error("Algorithm not set. InstanceMS_CFLP_CI::solve");
  return algorithm_->solve(this);
}

// Getters
const short InstanceMS_CFLP_CI::getNumWarehouses() const {
  return static_cast<short>(capacity_.size());
}

const short InstanceMS_CFLP_CI::getNumStores() const {
  return static_cast<short>(good_.size());
}  

const std::vector<short>& InstanceMS_CFLP_CI::getCapacity() const {
  return capacity_;
}

const std::vector<short>& InstanceMS_CFLP_CI::getFixedCost() const {
  return fixed_cost_;
}

const std::vector<short>& InstanceMS_CFLP_CI::getGood() const {
  return good_;
}

const std::vector<std::vector<short>>& InstanceMS_CFLP_CI::getSupplyCost() const {
  return supply_cost_;
}

const short InstanceMS_CFLP_CI::getNumIncompatibilities() const {
  return static_cast<short>(incompatible_pairs_.size());
}

const std::vector<ShortPair>& InstanceMS_CFLP_CI::getIncompatiblePairs() const {
  return incompatible_pairs_;
}