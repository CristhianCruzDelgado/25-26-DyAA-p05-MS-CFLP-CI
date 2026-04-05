/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef ALGORITHM_TOOLS_H_
#define ALGORITHM_TOOLS_H_

#include "my-types.h"

#include <algorithm>
#include <vector>

class AlgorithmTools {
 public:
  static std::vector<short> sortFixedCostAscending(const std::vector<short>&, const short&);
  static std::vector<short> sortSupplyCostAscending(std::vector<short>, const short&, const std::vector<short>&);
  static bool verifyCompatibility(const std::vector<ShortPair>&, const short&, const std::vector<short>&);  
};

#endif