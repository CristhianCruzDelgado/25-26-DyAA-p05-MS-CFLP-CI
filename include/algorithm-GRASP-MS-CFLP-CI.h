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
#include "algorithm.tools.h"

#include <stdexcept>
#include <random>
#include <vector>

class InstanceMSCFLPCI;
class SolutionMSCFLPCI;
class LocalSearch;

class AlgorithmGraspMSCFLPCI : public Algorithm {
 public:
  AlgorithmGraspMSCFLPCI(
    const short&, 
    const short&,
    const short&,
    const LocalSearch*
  );
  ~AlgorithmGraspMSCFLPCI() override = default;
  Solution* solve(const Instance*) const override;
  
 private:
  short grasp_iterations_;
  short rcl_size_;
  short slack_;

  SolutionMSCFLPCI* construct(const InstanceMSCFLPCI*) const;
  float objective_value_construct_solution_;
  const LocalSearch* local_search_;
};

#endif