/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

class Instance;
class Solution;

class Algorithm {
 public:
  virtual ~Algorithm() = default;
  virtual Solution* solve(const Instance*) const = 0;
};

#endif