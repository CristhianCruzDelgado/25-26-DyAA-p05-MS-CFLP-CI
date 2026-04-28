/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef ALGORITHM_GVNS_MS_CFLP_CI_H_
#define ALGORITHM_GVNS_MS_CFLP_CI_H_

#include "algorithm.h"

class Algorithm;
class LocalSearch;
class Shaking;

class AlgorithmGvnsMSCFLPCI : public Algorithm {
 public:
  AlgorithmGvnsMSCFLPCI(
    const short&,
    const short&,
    const Algorithm*,
    const Shaking*,
    const LocalSearch*
  );
  ~AlgorithmGvnsMSCFLPCI() override;
  Solution* solve(const Instance*) const override;
  
 private:
  short shaking_max_distance_;
  short gvns_iterations_;
  const Algorithm* initial_solution_constructor_;
  const Shaking* shaking_;
  const LocalSearch* local_search_;
};

#endif
