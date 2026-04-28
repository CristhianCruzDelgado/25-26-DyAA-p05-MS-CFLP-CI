/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>

class Timer {
 public:
  void begin();
  void end();
  double elapsed() const;

 private:
  std::chrono::high_resolution_clock::time_point begin_time_;
  std::chrono::high_resolution_clock::time_point end_time_;
};

#endif
