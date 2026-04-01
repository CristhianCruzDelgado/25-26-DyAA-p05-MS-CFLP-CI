/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef TABLE8_H_
#define TABLE8_H_

#include "table.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

#define RESET   "\033[0m"
#define BOLD    "\033[1m"

class Solution;

class Table8 : public Table {
 public:
  Table8(const Solution*);
  ~Table8() override = default;
  void displayOnConsole() const override;

 private:
  const Solution* solution_;
};

#endif