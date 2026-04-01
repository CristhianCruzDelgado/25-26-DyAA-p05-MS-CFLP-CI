/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef TABLE_H_
#define TABLE_H_

class Table {
 public:
  virtual ~Table() = default;
  virtual void displayOnConsole() const = 0;
};

#endif