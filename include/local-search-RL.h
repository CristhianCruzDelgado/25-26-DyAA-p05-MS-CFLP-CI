/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef LOCAL_SEARCH_RL_H_
#define LOCAL_SEARCH_RL_H_

#include "local-search.h"

#include <random>
#include <stdexcept>
#include <vector>

class LocalSearchRl : public LocalSearch {
 public:
  LocalSearchRl(
    const float&,
    const float&,
    const short&
  );
  ~LocalSearchRl() override;
  SolutionMSCFLPCI* solve(const SolutionMSCFLPCI*) const override;
  
 private:
  std::vector<LocalSearch*> local_searchs_;

  float epsilon_;
  float learning_rate_;
  short iterations_;
};

#endif
