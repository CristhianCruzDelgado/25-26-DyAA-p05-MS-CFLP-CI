/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef LOADER_H_
#define LOADER_H_

class Instance;

class Loader {
 public:
  virtual ~Loader() = default;
  virtual Instance* load(const char*) const = 0;
};

#endif