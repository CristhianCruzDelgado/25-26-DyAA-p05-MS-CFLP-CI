/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef ALGORITHM_CONSTRUCTOR_H_
#define ALGORITHM_CONSTRUCTOR_H_

class Algorithm;

class AlgorithmsConstructor {
 public:
  AlgorithmsConstructor(
    const char&,
    const char&
  );
  const Algorithm* createAlgorithm() const;

 private:
  char mode_;
  char option_;
};

#endif
