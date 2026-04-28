/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#include "../include/algorithm.h"
#include "../include/algorithms-constructor.h"
#include "../include/instance.h"
#include "../include/loader-MS-CFLP-CI.h"
#include "../include/solution.h"
#include "../include/timer.h"
#include "../include/tools.h"

#include <iostream>
#include <stdexcept>
#include <string>

const char* HELP_MESSAGE = "\nHelp: This program decide which warehouses to open \
and how to assign stores.\
\nTry:\
\n./MS-CFLP-CI-solver\
\n    [../Instances_MS-CFLP-CI/wlp[1..20].dzn ~ Run\
\n        [--greedy|--grasp [--shift|--swap-s|--swap-w|--incompatibility]|--gvns [--vnd|--rvnd|--rl]]]\
\n    [-h|--help]                             ~ Help\
\n    [-v|--version]                          ~ Version\n";

const char* VERSION = "MS-CFLP-CI Solver Version 1.2.0\n";

void solveMSCFLPCI(
  const char*, 
  const char&, 
  const char&
);

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
        } else if (std::string(argv[3]) == "--incompatibility") {
          solveMSCFLPCI(argv[1], '2', '4');
        } else {
          throw std::invalid_argument("Invalid algorithm option argument");
        }
      } else if (std::string(argv[2]) == "--gvns") {
        if (std::string(argv[3]) == "--vnd") {
          solveMSCFLPCI(argv[1], '3', '1');
        } else if (std::string(argv[3]) == "--rvnd") {
          solveMSCFLPCI(argv[1], '3', '2');
        } else if (std::string(argv[3]) == "--rl") {
          solveMSCFLPCI(argv[1], '3', '3');
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

void solveMSCFLPCI(
  const char* input_filename,
  const char& mode, 
  const char& option
) {
  /* Instance */
  const LoaderMSCFLPCI loader;
  Instance* instance = loader.load(input_filename);
  //printInstanceMSCFLPCI(instance);

  /* Algorithm */
  const AlgorithmsConstructor algorithms_constructor(mode, option);
  const Algorithm* algorithm = algorithms_constructor.createAlgorithm();
  instance->setAlgorithm(algorithm);

  /* Solution */
  Timer timer;
  timer.begin();
  Solution* solution = instance->solve();
  timer.end();
  solution->setTime(timer.elapsed());
  printSolutionMSCFLPCI(solution);
  printResultsMSCFLPCI(input_filename, solution);

  delete instance;
  delete algorithm;
  delete solution;
}
