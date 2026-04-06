/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#include "../include/algorithm-GRASP-MS-CFLP-CI.h"
#include "../include/algorithm-greedy-MS-CFLP-CI.h"
#include "../include/instance.h"
#include "../include/loader-MS-CFLP-CI.h"
#include "../include/local-search-ban-incompatibility.h"
#include "../include/local-search-shift.h"
#include "../include/local-search-swap-s.h"
#include "../include/local-search-swap-w.h"
#include "../include/solution.h"
#include "../include/table1.h"
#include "../include/table2.h"
#include "../include/table4.h"
#include "../include/table5.h"
#include "../include/table6.h"
#include "../include/table7.h"
#include "../include/table8.h"
#include "../include/timer.h"

#include <iostream>
#include <stdexcept>
#include <string>

const char* HELP_MESSAGE = "\nHelp: This program decide which warehouses to open\
and how to assign stores.\
\nTry:\
\n./MS-CFLP-CI-solver\
\n    [../Instances_MS-CFLP-CI/wlp[1..20].dzn ~ Run\
\n        [--greedy|--grasp [--shift|--swap-s|--swap-w|--ban-incompatibilities]]]\
\n    [-h|--help]                             ~ Help\
\n    [-v|--version]                          ~ Version\n";

const char* VERSION = "MS-CFLP-CI Solver Version 1.1.0\n";

void solveMSCFLPCI(const char*, const char&, const char&);
void printInstanceMSCFLPCI(const Instance*);
void printSolutionMSCFLPCI(const Solution*);

int main(int argc, char* argv[]) {
  try {
    if (argc == 2) {
      if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
        std::cout << HELP_MESSAGE; 
        return 0;
      } else if (std::string(argv[1]) == "-v" || std::string(argv[1]) == "--version") {
        std::cout << VERSION;
        return 0;
      } else {
        throw std::invalid_argument("Invalid number of arguments");
      }
    } else if (argc == 3) {
      if (std::string(argv[2]) == "--greedy") {
        solveMSCFLPCI(argv[1], '1', '0');
      } else {
        throw std::invalid_argument("Invalid algorithm argument");
      }
    } else if (argc == 4) {
      if (std::string(argv[2]) == "--grasp") {
        if (std::string(argv[3]) == "--shift") {
          solveMSCFLPCI(argv[1], '2', '1');
        } else if (std::string(argv[3]) == "--swap-s") {
          solveMSCFLPCI(argv[1], '2', '2');
        } else if (std::string(argv[3]) == "--swap-w") {
          solveMSCFLPCI(argv[1], '2', '3');
        } else if (std::string(argv[3]) == "--ban-incompatibility") {
          solveMSCFLPCI(argv[1], '2', '4');
        } else {
          throw std::invalid_argument("Invalid algorithm option argument");
        }
      } else {
        throw std::invalid_argument("Invalid algorithm argument");
      }
    } else {
      throw std::invalid_argument("Invalid number of arguments");
    }
  } catch (const std::exception& e) {
    std::cerr << "\nError: " << e.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "\nError: ???\n";
    return 2;
  }
  return 0;
}

void solveMSCFLPCI(const char* input_file, const char& mode, const char& options) {
  const LoaderMSCFLPCI* loader = new LoaderMSCFLPCI();
  Instance* instance = loader->load(input_file);
  printInstanceMSCFLPCI(instance);
  const Algorithm* algorithm = nullptr;
  const short SLACK_FOR_COMPATIBILITY = 5;
  const short GRASP_ITERATIONS = 30;
  const short RCL_SIZE = 3;
  const LocalSearch* local_search = nullptr;
  switch (mode) {
    case '1': { algorithm = new AlgorithmGreedyMSCFLPCI(SLACK_FOR_COMPATIBILITY); break; }
    case '2': {
      switch (options) {
        case '1': local_search = new LocalSearchShift(); break;
        case '2': local_search = new LocalSearchSwapS(); break;
        case '3': local_search = new LocalSearchSwapW(); break;
        case '4': local_search = new LocalSearchBanIncompatibility(); break;
      }
      algorithm = new AlgorithmGraspMSCFLPCI(GRASP_ITERATIONS, RCL_SIZE, SLACK_FOR_COMPATIBILITY, local_search); break;
    }
  }
  instance->setAlgorithm(algorithm);
  Timer* timer = new Timer();
  timer->begin();
  Solution* solution = instance->solve();
  timer->end();
  solution->setTime(timer->elapsed());
  printSolutionMSCFLPCI(solution);

  delete loader;
  delete instance;
  delete timer;
  delete solution;
}

void printInstanceMSCFLPCI(const Instance* instance) {
  Table* table = new Table1(instance);
  table->displayOnConsole();
  table = new Table2(instance);
  table->displayOnConsole();
  // TODO: add #include "../include/table3.h"
  // table = new Table3(instance);
  // table->displayOnConsole();
  delete table;

  std::cout << "\n\n - - - - - - - - - - \n";
}

void printSolutionMSCFLPCI(const Solution* solution) {
  std::cout << "\n - - - - - - - - - - \n\n";

  Table* table = new Table4(solution);
  table->displayOnConsole();
  table = new Table5(solution);
  table->displayOnConsole();
  table = new Table6(solution);
  table->displayOnConsole();
  table = new Table7(solution);
  table->displayOnConsole();
  table = new Table8(solution);
  table->displayOnConsole();
  delete table;  
}
