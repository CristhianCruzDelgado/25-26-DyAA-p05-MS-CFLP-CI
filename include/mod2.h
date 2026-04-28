/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef MOD2_H_
#define MOD2_H_

#include "local-search.h"

#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

class LocalSearchRvndMod : public LocalSearch {
 public:
  LocalSearchRvndMod();
  ~LocalSearchRvndMod() override;
  SolutionMSCFLPCI* solve(const SolutionMSCFLPCI*) const override;
  
 private:
  std::vector<LocalSearch*> local_searchs_;
};

#endif
