/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef INSTANCE_MS_CFLP_CI_H_
#define INSTANCE_MS_CFLP_CI_H_

#include "instance.h"
#include "my-types.h"

#include <stdexcept>
#include <vector>

class InstanceMSCFLPCI : public Instance {
 public:
  InstanceMSCFLPCI(
    const std::vector<short>&, 
    const std::vector<short>&,
    const std::vector<short>&, 
    const std::vector<std::vector<short>>&, 
    const std::vector<ShortPair>&);
  ~InstanceMSCFLPCI() override;

  void setAlgorithm(const Algorithm*) override;
  Solution* solve() const override;

  // getters
  const short getNumWarehouses() const;
  const short getNumStores() const;
  const std::vector<short>& getCapacity() const;
  const std::vector<short>& getFixedCost() const;
  const std::vector<short>& getGood() const;
  const std::vector<std::vector<short>>& getSupplyCost() const;
  const short getNumIncompatibilities() const;
  const std::vector<ShortPair>& getIncompatiblePairs() const;

 private:
  std::vector<short> capacity_;
  std::vector<short> fixed_cost_;
  std::vector<short> good_;
  std::vector<std::vector<short>> supply_cost_;
  std::vector<ShortPair> incompatible_pairs_;
  const Algorithm* algorithm_;
};

#endif