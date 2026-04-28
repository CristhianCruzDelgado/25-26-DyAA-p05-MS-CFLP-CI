#include "../include/local-search-VND.h"
#include "../include/solution-MS-CFLP-CI.h"
#include "../include/local-search-ban-incompatibility.h"
#include "../include/local-search-shift.h"
#include "../include/local-search-swap-store.h"
#include "../include/local-search-swap-warehouse.h"

LocalSearchVnd::LocalSearchVnd() {
  local_searchs_.push_back(new LocalSearchShift());
  local_searchs_.push_back(new LocalSearchSwapS());
  local_searchs_.push_back(new LocalSearchSwapW());
  local_searchs_.push_back(new LocalSearchBanIncompatibility());
}

LocalSearchVnd::~LocalSearchVnd() {
  short num_local_searchs_ = local_searchs_.size();
  for (short i = 0; i < num_local_searchs_; ++i) {
    delete local_searchs_[i];
  }
}

SolutionMSCFLPCI* LocalSearchVnd::solve(const SolutionMSCFLPCI* initial_solution) const {
  if (initial_solution == nullptr) throw std::invalid_argument("Invalid solution parameter. LocalSearchVnd::solve");
  SolutionMSCFLPCI* current_solution = new SolutionMSCFLPCI(*initial_solution);

  short i = 0;
  while (i < local_searchs_.size()) {
    LocalSearch* local_search = local_searchs_[i];
    SolutionMSCFLPCI* improved_solution = local_search->solve(current_solution);
    float improved_value = improved_solution->getObjectiveValue();
    float current_value = current_solution->getObjectiveValue();
    if (improved_value < current_value) {
      delete current_solution;
      current_solution = improved_solution;
      i = 0;
    } else {
      delete improved_solution;
      ++i;
    }
  }

  return current_solution;
}
