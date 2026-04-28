#include "../include/tools.h"
#include "../include/table1.h"
#include "../include/table2.h"
#include "../include/table4.h"
#include "../include/table5.h"
#include "../include/table6.h"
#include "../include/table7.h"
#include "../include/table8.h"
#include "../include/table9.h"
#include "../include/table10.h"

void printInstanceMSCFLPCI(const Instance* instance) {
  Table* table = new Table1(instance);
  table->displayOnConsole();
  delete table;
  table = new Table2(instance);
  table->displayOnConsole();
  // TODO: add #include "../include/table3.h"
  // table = new Table3(instance);
  // table->displayOnConsole();
  delete table;
}

void printSolutionMSCFLPCI(const Solution* solution) {
  Table* table = new Table4(solution);
  table->displayOnConsole();
  delete table;
  table = new Table5(solution);
  table->displayOnConsole();
  delete table;
  table = new Table6(solution);
  table->displayOnConsole();
  delete table;
  table = new Table7(solution);
  table->displayOnConsole();
  delete table;
  table = new Table8(solution);
  table->displayOnConsole();
  delete table;  
}

void printResultsMSCFLPCI(const char* input_filename, const Solution* solution) {
  Table* table = new Table9(input_filename, solution);
  table->displayOnConsole();
  delete table;
  table = new Table10(input_filename, solution);
  table->displayOnConsole();
  delete table;
}
