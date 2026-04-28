/**
 * Universidad de La Laguna ULL
 * Degree: Computer Engineering
 * Signature: Design and Analysis of Algorithms
 * Practice 05: MS-CFLP-CI
 * Author: Cristhian Adoney Cruz Delgado
 * Email: alu0101648293@ull.edu.es
 */

#ifndef LOADER_MS_CFLP_CI_H_
#define LOADER_MS_CFLP_CI_H_

#include "loader.h"
#include "my-types.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class Instance;

class LoaderMSCFLPCI : public Loader {
 public:
  ~LoaderMSCFLPCI() override = default;
  Instance* load(const char*) const override;

 private:
  StringPair readUntilSemicolon(std::ifstream&, const std::string&) const;
  short readShort(const std::string&) const;
  std::vector<short> readVectorShort(const std::string&, short) const;
  std::vector<std::vector<short>> loadMatrix(std::ifstream&, const std::string&, short, short) const;
  int readInt(const std::string&) const;
  std::vector<ShortPair> readVectorPairsShort(const std::string&, short) const;
};

#endif
