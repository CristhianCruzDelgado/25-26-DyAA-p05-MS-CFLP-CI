#include "../include/mod2.h"
#include "../include/solution-MS-CFLP-CI.h"
#include "../include/local-search-shift.h"
#include "../include/local-search-swap-warehouse.h"

LocalSearchRvndMod::LocalSearchRvndMod() {
  local_searchs_.push_back(new LocalSearchShift());
  local_searchs_.push_back(new LocalSearchSwapW());
}

LocalSearchRvndMod::~LocalSearchRvndMod() {
  short num_local_searchs = local_searchs_.size();
  for (short i = 0; i < num_local_searchs; ++i) {
    delete local_searchs_[i];
  }
}

SolutionMSCFLPCI* LocalSearchRvndMod::solve(const SolutionMSCFLPCI* initial_solution) const {
  if (initial_solution == nullptr) throw std::invalid_argument("Invalid solution parameter. LocalSearchRvndMod::solve");
  SolutionMSCFLPCI* current_solution = new SolutionMSCFLPCI(*initial_solution);
  std::random_device rd;
  std::mt19937 gen(rd());

  std::vector<LocalSearch*> active_local_searchs = local_searchs_;
  short no_improve_iterations = 0;
  while (!active_local_searchs.empty() && no_improve_iterations < 10) {
    std::uniform_int_distribution<short> dist(0, active_local_searchs.size() - 1);
    short pos = dist(gen);
    LocalSearch* local_search = active_local_searchs[pos];
    SolutionMSCFLPCI* improved_solution = local_search->solve(current_solution);
    float improved_value = improved_solution->getObjectiveValue();
    float current_value = current_solution->getObjectiveValue();
    if (improved_value < current_value) {
      delete current_solution;
      current_solution = improved_solution;
      active_local_searchs = local_searchs_;
      no_improve_iterations = 0;
    } else {
      delete improved_solution;
      active_local_searchs.erase(active_local_searchs.begin() + pos);
      ++no_improve_iterations;
    }
  }

  return current_solution;
}
