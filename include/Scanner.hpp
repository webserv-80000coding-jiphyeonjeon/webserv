// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef SCANNER_HPP_
#define SCANNER_HPP_

#include <string>
#include <vector>

class Scanner {
 public:
  typedef std::vector<std::string> TokensType;

  Scanner(const std::string& filename);

  const TokensType& getTokens() const;

 private:
  TokensType tokens_;

  void readFile(const std::string& filename);
  void tokenize(const std::string& str);
  void checkMetaCharacter(const std::string& str);
};

std::ostream& operator<<(std::ostream& os, const Scanner::TokensType& tokens);

#endif  // SCANNER_HPP_
