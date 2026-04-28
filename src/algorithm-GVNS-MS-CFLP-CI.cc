#include "../include/algorithm-GVNS-MS-CFLP-CI.h"
#include "../include/algorithm-GRASP-MS-CFLP-CI.h"
#include "../include/algorithm-random-greedy-MS-CFLP-CI.h"
#include "../include/instance-MS-CFLP-CI.h"
#include "../include/solution-MS-CFLP-CI.h"
#include "../include/local-search.h"
#include "../include/local-search-RVND.h"
#include "../include/shaking.h"

AlgorithmGvnsMSCFLPCI::AlgorithmGvnsMSCFLPCI(
  const short& shaking_max_distance,
  const short& gvns_iterations,
  const Algorithm* initial_solution_constructor,
  const Shaking* shaking,
  const LocalSearch* local_search
) : 
  shaking_max_distance_(shaking_max_distance),
  gvns_iterations_(gvns_iterations),
  initial_solution_constructor_(initial_solution_constructor),
  shaking_(shaking),
  local_search_(local_search) {}

AlgorithmGvnsMSCFLPCI::~AlgorithmGvnsMSCFLPCI() {
  delete initial_solution_constructor_;
  delete shaking_;
  delete local_search_;
}

Solution* AlgorithmGvnsMSCFLPCI::solve(const Instance* instance) const {
  if (instance == nullptr) throw std::invalid_argument("Invalid instance parameter. AlgorithmGvnsMSCFLPCI::solve");
  if (initial_solution_constructor_ == nullptr) throw std::invalid_argument("Algorithm not set. AlgorithmGvnsMSCFLPCI::solve");
  if (shaking_ == nullptr) throw std::invalid_argument("Shaking not set. AlgorithmGvnsMSCFLPCI::solve");
  if (local_search_ == nullptr) throw std::runtime_error("LocalSearch not set. AlgorithmGvnsMSCFLPCI::solve");
  Solution* initial_solution = initial_solution_constructor_->solve(instance);
  SolutionMSCFLPCI* solution_MSCFLPCI = dynamic_cast<SolutionMSCFLPCI*>(initial_solution);
  if (solution_MSCFLPCI == nullptr) throw std::invalid_argument("Invalid solution type. AlgorithmGvnsMSCFLPCI::solve");
  LocalSearch* rvnd = new LocalSearchRvnd();
  SolutionMSCFLPCI* construct_solution = rvnd->solve(solution_MSCFLPCI);
  delete rvnd;
  delete solution_MSCFLPCI;

  SolutionMSCFLPCI* best_solution = construct_solution;
  for (int i = 0; i < gvns_iterations_; ++i) {
    short k = 1;
    while (k <= shaking_max_distance_) {
      SolutionMSCFLPCI* shaked_solution = shaking_->shake(construct_solution, k);
      SolutionMSCFLPCI* improved_solution = local_search_->solve(shaked_solution);
      delete shaked_solution;
      const float improved_value = improved_solution->getObjectiveValue();
      const float best_value     = best_solution->getObjectiveValue();
      if (improved_value < best_value) {
        if (construct_solution != best_solution) delete construct_solution;
        delete best_solution;
        best_solution = improved_solution;
        construct_solution = improved_solution;   
        k = 1;

        const float total_improve = ((best_value - improved_value) / best_value) * 100.0f;
        std::cout << best_value << "->" << improved_value << ". Improved: " << total_improve << "%\n"; // verbose
      } else {
        ++k;
        delete improved_solution;
      }
    }
    std::cout << "GVNS iteration " << i + 1 << "/" << gvns_iterations_ << " completed.\n"; // verbose
  }
  std::cout << "GVNS completed.\n"; // verbose

  if (construct_solution != best_solution) delete construct_solution;
  return best_solution;
}
