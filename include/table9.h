/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef TABLE9_H_
#define TABLE9_H_

#include "table.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#define RESET   "\033[0m"
#define BOLD    "\033[1m"

class Solution;

class Table9 : public Table {
 public:
  Table9(const char*, const Solution*);
  ~Table9() override = default;
  void displayOnConsole() const override;

 private:
  const char* input_file_;
  const Solution* solution_;
};

#endif
