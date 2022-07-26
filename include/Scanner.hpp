// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef SCANNER_HPP_
#define SCANNER_HPP_

#include <string>
#include <vector>

class Scanner {
 public:
  typedef std::vector<std::string> TokensType;

  static void readFile(TokensType& tokens, const std::string& filename);
  static void tokenize(TokensType& tokens, const std::string& str);
  static void checkMetaCharacter(TokensType& tokens, const std::string& str);
};

std::ostream& operator<<(std::ostream& os, const Scanner::TokensType& tokens);

#endif  // SCANNER_HPP_
