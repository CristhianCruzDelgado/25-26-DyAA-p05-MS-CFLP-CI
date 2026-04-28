/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef SHAKING_H_
#define SHAKING_H_

class SolutionMSCFLPCI;

class Shaking {
 public:
  virtual ~Shaking() = default;
  virtual SolutionMSCFLPCI* shake(const SolutionMSCFLPCI*, const short&) const = 0;
};

#endif
