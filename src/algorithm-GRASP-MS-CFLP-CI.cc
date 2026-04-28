#include "../include/algorithm-GRASP-MS-CFLP-CI.h"
#include "../include/algorithm.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"
#include "../include/local-search.h"

AlgorithmGraspMSCFLPCI::AlgorithmGraspMSCFLPCI(
  const short& grasp_iterations,
  const Algorithm* solution_constructor,
  const LocalSearch* local_search
) :
  grasp_iterations_(grasp_iterations),
  solution_constructor_(solution_constructor),
  local_search_(local_search) {}

AlgorithmGraspMSCFLPCI::~AlgorithmGraspMSCFLPCI() {
  delete solution_constructor_;
  delete local_search_;
}

Solution* AlgorithmGraspMSCFLPCI::solve(const Instance* instance) const {
  if (instance == nullptr) throw std::invalid_argument("Invalid instance parameter. AlgorithmGraspMSCFLPCI::solve");
  const InstanceMSCFLPCI* instance_MSCFLPCI = dynamic_cast<const InstanceMSCFLPCI*>(instance);
  if (instance_MSCFLPCI == nullptr) throw std::invalid_argument("Invalid instance type. AlgorithmGraspMSCFLPCI::solve");
  if (local_search_ == nullptr) throw std::runtime_error("LocalSearch not set. AlgorithmGraspMSCFLPCI::solve");

  SolutionMSCFLPCI* best_solution = nullptr;

  for (short i = 0; i < grasp_iterations_; ++i) {
    Solution* solution = solution_constructor_->solve(instance_MSCFLPCI);
    SolutionMSCFLPCI* construct_solution = dynamic_cast<SolutionMSCFLPCI*>(solution);
    if (construct_solution == nullptr) throw std::invalid_argument("Invalid solution type. AlgorithmGraspMSCFLPCI::solve");
    SolutionMSCFLPCI* improved_solution = local_search_->solve(construct_solution);
    float construct_value = construct_solution->getObjectiveValue();
    float improved_value = improved_solution->getObjectiveValue();
    delete construct_solution;
    if (best_solution == nullptr || improved_value < best_solution->getObjectiveValue()) {
      delete best_solution;
      best_solution = improved_solution;
      
      float total_improve = ((construct_value - improved_value) / improved_value) * 100.0f;
      std::cout << construct_value << "->" << improved_value << ". Improved: " << total_improve << "%\n"; // verbose
    } else {
      delete improved_solution;
    }
    std::cout << "GRASP iteration " << i + 1 << "/" << grasp_iterations_ << " completed.\n"; // verbose
  }
  std::cout << "GRASP completed.\n"; // verbose

  return best_solution;
}
