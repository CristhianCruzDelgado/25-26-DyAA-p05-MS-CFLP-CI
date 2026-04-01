/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef TABLE7_H_
#define TABLE7_H_

#include "table.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

#define RESET   "\033[0m"
#define BOLD    "\033[1m"

class Solution;

class Table7 : public Table {
 public:
  Table7(const Solution*);
  ~Table7() override = default;
  void displayOnConsole() const override;

 private:
  const Solution* solution_;
};

#endif