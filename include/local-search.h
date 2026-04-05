/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef LOCAL_SEARCH_H_
#define LOCAL_SEARCH_H_

class SolutionMSCFLPCI;

class LocalSearch {
 public:
  virtual ~LocalSearch() = default;
  virtual SolutionMSCFLPCI* solve(SolutionMSCFLPCI*) const = 0;
};

#endif