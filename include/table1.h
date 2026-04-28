/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef TABLE1_H_
#define TABLE1_H_

#include "table.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

#define RESET   "\033[0m"
#define BOLD    "\033[1m"

class Instance;

class Table1 : public Table {
 public:
  Table1(const Instance*);
  ~Table1() override = default;
  void displayOnConsole() const override;

 private:
  const Instance* instance_;
};

#endif
