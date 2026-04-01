/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_

class Solution {
 public:
  virtual ~Solution() = default;
  virtual void setTime(double) = 0;
  virtual float objectiveFunction() const = 0;
};

#endif