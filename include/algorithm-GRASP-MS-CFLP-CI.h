/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef ALGORITHM_GRASP_MS_CFLP_CI_H_
#define ALGORITHM_GRASP_MS_CFLP_CI_H_

#include "algorithm.h"

#include <stdexcept>

class AlgorithmGRASPMS_CFLP_CI : public Algorithm {
 public:
  ~AlgorithmGRASPMS_CFLP_CI() override = default;
  Solution* solve(const Instance*) const override;
};

#endif