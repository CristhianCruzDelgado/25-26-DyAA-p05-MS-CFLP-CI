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

const char* HELP_MESSAGE = "\nHelp: This program decide which warehouses to open and how to assign stores. \
                            \nTry: ./MS-CFLP-CI-solver                                  ~ Executable \
                            \n../Instances_MS-CFLP-CI/wlp[1..20].dzn [--greedy|--grasp] ~ Run \
                            \n[-h|--help]                                               ~ Help \
                            \n[-v|--version]                                            ~ Version\n";

const char* VERSION = "MS-CFLP-CI Solver Version 1.0.0\n";

void solve_MS_CFLP_CI(const char*, const char&);
void printInstanceMS_CFLP_CI(const Instance*);
void printSolutionMS_CFLP_CI(const Solution*);

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
        solve_MS_CFLP_CI(argv[1], '1');
      } else if (std::string(argv[2]) == "--grasp") {
        solve_MS_CFLP_CI(argv[1], '2');
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

void solve_MS_CFLP_CI(const char* input_file, const char& mode) {
  const LoaderMS_CFLP_CI* loader = new LoaderMS_CFLP_CI();
  Instance* instance = loader->load(input_file);
  printInstanceMS_CFLP_CI(instance);
  const Algorithm* algorithm = nullptr;
  const short slack_for_incompatibilities = 5;
  switch (mode) {
    case '1': algorithm = new AlgorithmGreedyMS_CFLP_CI(slack_for_incompatibilities); break;
    case '2': algorithm = new AlgorithmGRASPMS_CFLP_CI(); break;
    // TODO: add case '3', case '4', ...
  }
  instance->setAlgorithm(algorithm);
  Timer* timer = new Timer();
  timer->begin();
  Solution* solution = instance->solve();
  timer->end();
  solution->setTime(timer->elapsed());
  printSolutionMS_CFLP_CI(solution);

  delete loader;
  delete instance;
  delete timer;
  delete solution;
}

void printInstanceMS_CFLP_CI(const Instance* instance) {
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

void printSolutionMS_CFLP_CI(const Solution* solution) {
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
