/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef SHAKING_SHIFT_H_
#define SHAKING_SHIFT_H_

#include "algorithm.tools.h"
#include "shaking.h"

#include <cmath>
#include <random>
#include <stdexcept>
#include <vector>

class ShakingShift : public Shaking {
 public:
  ~ShakingShift() override = default;
  SolutionMSCFLPCI* shake(const SolutionMSCFLPCI*, const short&) const override;
};

#endif
