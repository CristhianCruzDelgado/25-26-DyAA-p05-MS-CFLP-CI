/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef LOCAL_SEARCH_SWAP_W_H_
#define LOCAL_SEARCH_SWAP_W_H_

#include "algorithm.tools.h"
#include "local-search.h"

class LocalSearchSwapW : public LocalSearch {
 public:
  ~LocalSearchSwapW() override = default;
  SolutionMSCFLPCI* solve(SolutionMSCFLPCI*) const override;
};

#endif