#include "timer.h"

void Timer::begin() {
  begin_time_ = std::chrono::high_resolution_clock::now();
}

void Timer::end() {
  end_time_ = std::chrono::high_resolution_clock::now();
}

double Timer::elapsed() const {
  return std::chrono::duration<double>(end_time_ - begin_time_).count();
}
