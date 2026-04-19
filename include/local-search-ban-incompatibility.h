/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef LOCAL_SEARCH_BAN_INCOMPATIBILITY_H_
#define LOCAL_SEARCH_BAN_INCOMPATIBILITY_H_

#include "algorithm.tools.h"
#include "local-search.h"

#include <cmath>
#include <stdexcept>

class LocalSearchBanIncompatibility : public LocalSearch {
 public:
  ~LocalSearchBanIncompatibility() override = default;
  SolutionMSCFLPCI* solve(const SolutionMSCFLPCI*) const override;
};

#endif