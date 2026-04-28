/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef LOCAL_SEARCH_VND_H_
#define LOCAL_SEARCH_VND_H_

#include "local-search.h"

#include <iostream>
#include <stdexcept>
#include <vector>

class LocalSearchVnd : public LocalSearch {
 public:
  LocalSearchVnd();
  ~LocalSearchVnd() override;
  SolutionMSCFLPCI* solve(const SolutionMSCFLPCI*) const override;
  
 private:
  std::vector<LocalSearch*> local_searchs_;
};

#endif
