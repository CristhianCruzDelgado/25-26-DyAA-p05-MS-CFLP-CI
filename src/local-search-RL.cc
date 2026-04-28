#include "../include/local-search-RL.h"
#include "../include/solution-MS-CFLP-CI.h"
#include "../include/local-search-ban-incompatibility.h"
#include "../include/local-search-shift.h"
#include "../include/local-search-swap-store.h"
#include "../include/local-search-swap-warehouse.h"

LocalSearchRl::LocalSearchRl(
  const float& epsilon,
  const float& learning_rate,
  const short& iterations
) :
  epsilon_(epsilon),
  learning_rate_(learning_rate),
  iterations_(iterations)
{
  local_searchs_.push_back(new LocalSearchShift());
  local_searchs_.push_back(new LocalSearchSwapS());
  local_searchs_.push_back(new LocalSearchSwapW());
  local_searchs_.push_back(new LocalSearchBanIncompatibility());
}

LocalSearchRl::~LocalSearchRl() {
  short num_local_searchs_ = local_searchs_.size();
  for (short i = 0; i < num_local_searchs_; ++i) {
    delete local_searchs_[i];
  }
}

SolutionMSCFLPCI* LocalSearchRl::solve(const SolutionMSCFLPCI* initial_solution) const {
  if (initial_solution == nullptr) throw std::invalid_argument("Invalid solution parameter. LocalSearchRl::solve");
  SolutionMSCFLPCI* current_solution = new SolutionMSCFLPCI(*initial_solution);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist_f(0.0f, 1.0f);

  std::vector<float> confidence(local_searchs_.size(), 0.5f);
  short i = 0;
  while (i < iterations_) {
    float epsilon = dist_f(gen);
    short pos = 0;
    if (epsilon < epsilon_) {
      std::uniform_int_distribution<short> dist_s(0, local_searchs_.size() - 1);
      pos = dist_s(gen);
    } else {
      float max_confidence = -1.0f;
      for (short j = 0; j < confidence.size(); ++j) {
        if (confidence[j] > max_confidence) {
          max_confidence = confidence[j];
          pos = j;
        }
      }
    }
    LocalSearch* local_search = local_searchs_[pos];
    SolutionMSCFLPCI* improved_solution = local_search->solve(current_solution);
    float improved_value = improved_solution->getObjectiveValue();
    float current_value = current_solution->getObjectiveValue();
    if (improved_value < current_value) {
      delete current_solution;
      current_solution = improved_solution;
      confidence[pos] += learning_rate_ * (1.0f - confidence[pos]);
      i = 0;
    } else {
      delete improved_solution;
      confidence[pos] *= (1.0f - learning_rate_);
      ++i;
    }
  }

  return current_solution;
}
