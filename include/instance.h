/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef INSTANCE_H_
#define INSTANCE_H_

class Algorithm;
class Solution;

class Instance {
 public:
  virtual ~Instance() = default;
  virtual void setAlgorithm(const Algorithm*) = 0;
  virtual Solution* solve() = 0;
};

#endif