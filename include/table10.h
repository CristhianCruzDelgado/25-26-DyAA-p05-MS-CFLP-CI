/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef TABLE10_H_
#define TABLE10_H_

#include "table.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#define RESET   "\033[0m"
#define BOLD    "\033[1m"

class Solution;

class Table10 : public Table {
 public:
  Table10(const char*, const Solution*);
  ~Table10() override = default;
  void displayOnConsole() const override;

 private:
  const char* input_file_;
  const Solution* solution_;
};

#endif