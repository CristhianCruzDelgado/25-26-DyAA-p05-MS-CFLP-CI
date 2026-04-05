/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef SOLUTION_MS_CFLP_CI_H_
#define SOLUTION_MS_CFLP_CI_H_

#include "solution.h"

#include <vector>

class InstanceMSCFLPCI;

class SolutionMSCFLPCI : public Solution {
 public:
  SolutionMSCFLPCI(
    const InstanceMSCFLPCI*, 
    const std::vector<short>&, 
    const std::vector<std::vector<short>>&, 
    const std::vector<std::vector<float>>&, 
    const std::vector<short>&, 
    const std::vector<short>&
  );
  ~SolutionMSCFLPCI() override = default;
  void setTime(double) override;
  float objectiveFunction() const override;

  // getters
  const InstanceMSCFLPCI* getInstanceData() const;
  const short getNumWarehousesAssigned() const;
  const std::vector<short>& getWarehouseAssigned() const;
  const std::vector<std::vector<short>>& getAssignment() const;
  const std::vector<std::vector<float>>& getGoodSupplied() const;
  const std::vector<short>& getResidualCapacity() const;
  const std::vector<short>& getResidualGood() const;
  const double& getTime() const;
  const float& getFixedCostsSum() const;
  const float& getSupplyCostsSum() const;
  const float& getObjectiveValue() const;

 private:
  const InstanceMSCFLPCI* instance_data_;
  std::vector<short> warehouse_assigned_;
  std::vector<std::vector<short>> assignment_;
  std::vector<std::vector<float>> good_supplied_;
  std::vector<short> residual_capacity_;
  std::vector<short> residual_good_;
  double time_;
  float fixed_costs_sum_;
  float supply_costs_sum_;
  float objective_value_;

  float fixedCostSumFunction() const;
  float supplyCostSumFunction() const;
};

#endif