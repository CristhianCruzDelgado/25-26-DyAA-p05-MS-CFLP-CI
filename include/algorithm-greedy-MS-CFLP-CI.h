/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef ALGORITHM_GREEDY_MS_CFLP_CI_H_
#define ALGORITHM_GREEDY_MS_CFLP_CI_H_

#include "algorithm.h"
#include "my-types.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

class AlgorithmGreedyMS_CFLP_CI : public Algorithm {
 public:
  AlgorithmGreedyMS_CFLP_CI(const short& slack);
  ~AlgorithmGreedyMS_CFLP_CI() override = default;
  Solution* solve(const Instance*) const override;
 private:
  short slack_;

  std::vector<short> sortFixedCostAscending(const std::vector<short>&, const short&) const;
  std::vector<short> sortSupplyCostAscending(std::vector<short>, const short&, const std::vector<short>&) const;
  bool verifyCompatibility(const std::vector<ShortPair>&, const short&, const std::vector<short>&) const;
};

#endif