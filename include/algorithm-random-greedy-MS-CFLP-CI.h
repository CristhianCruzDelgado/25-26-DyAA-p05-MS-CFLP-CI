/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef ALGORITHM_RANDOM_GREEDY_MS_CFLP_CI_H_
#define ALGORITHM_RANDOM_GREEDY_MS_CFLP_CI_H_

#include "algorithm.h"
#include "algorithm.tools.h"

#include <algorithm>
#include <stdexcept>
#include <random>
#include <vector>

class AlgorithmRandomGreedyMSCFLPCI : public Algorithm {
 public:
  AlgorithmRandomGreedyMSCFLPCI(
    const short&,
    const short&
  );
  ~AlgorithmRandomGreedyMSCFLPCI() override = default;
  Solution* solve(const Instance*) const override;

 private:
  short slack_;
  short rcl_size_;
};

#endif
