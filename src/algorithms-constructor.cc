#include "../include/algorithms-constructor.h"
#include "../include/algorithm-GRASP-MS-CFLP-CI.h"
#include "../include/algorithm-greedy-MS-CFLP-CI.h"
#include "../include/algorithm-random-greedy-MS-CFLP-CI.h"
#include "../include/algorithm-GVNS-MS-CFLP-CI.h"
#include "../include/local-search-ban-incompatibility.h"
#include "../include/local-search-RL.h"
#include "../include/local-search-RVND.h"
#include "../include/local-search-shift.h"
#include "../include/local-search-swap-store.h"
#include "../include/local-search-swap-warehouse.h"
#include "../include/local-search-VND.h"
#include "../include/shaking-shift.h"

AlgorithmsConstructor::AlgorithmsConstructor(
  const char& mode,
  const char& option
) : 
  mode_(mode),
  option_(option) {}

const Algorithm* AlgorithmsConstructor::createAlgorithm() const {
  const short SLACK_FOR_COMPATIBILITY = 5;
  const short RCL_SIZE = 3;
  const short GRASP_ITERATIONS = 300;
  const short SHAKING_MAX_DISTANCE = 30;
  const short GVNS_ITERATIONS = 4; 
  const float EPSILON_RL = 0.15f;
  const float LEARNING_RATE = 0.3f;
  const short RL_ITERATIONS = 4;

  if (mode_ == '1') {
    return new AlgorithmGreedyMSCFLPCI(SLACK_FOR_COMPATIBILITY);
  }
  
  if (mode_ == '2') {
    const Algorithm* random_greedy = new AlgorithmRandomGreedyMSCFLPCI(SLACK_FOR_COMPATIBILITY, RCL_SIZE);
    const LocalSearch* local_search = nullptr;
    switch (option_) {
      case '1': local_search = new LocalSearchShift(); break;
      case '2': local_search = new LocalSearchSwapS(); break;
      case '3': local_search = new LocalSearchSwapW(); break;
      case '4': local_search = new LocalSearchBanIncompatibility(); break;
    }
    return new AlgorithmGraspMSCFLPCI(GRASP_ITERATIONS, random_greedy, local_search);
  }

  if (mode_ == '3') {
    const Algorithm* random_greedy = new AlgorithmRandomGreedyMSCFLPCI(SLACK_FOR_COMPATIBILITY, RCL_SIZE);
    const Shaking* shaking = new ShakingShift();
    LocalSearch* local_search = nullptr;
    switch (option_) {
      case '1': local_search = new LocalSearchVnd(); break;
      case '2': local_search = new LocalSearchRvnd(); break;
      case '3': local_search = new LocalSearchRl(EPSILON_RL, LEARNING_RATE, RL_ITERATIONS); break;
    }
    return new AlgorithmGvnsMSCFLPCI(SHAKING_MAX_DISTANCE, GVNS_ITERATIONS, random_greedy, shaking, local_search);
  }

  return nullptr;
}
