// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef SCANNER_HPP_
#define SCANNER_HPP_

#include <string>

class Scanner {
 public:
  Scanner();
  ~Scanner();

  std::string readFile(const std::string& filename);
};

#endif  // SCANNER_HPP_
