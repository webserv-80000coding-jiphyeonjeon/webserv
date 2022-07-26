// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "Config.hpp"
#include "Scanner.hpp"

class Parser {
 public:
  typedef Scanner::TokensType TokensType;
  typedef void (Parser::*ParserFuncType)(TokensType&);
  typedef std::map<std::string, ParserFuncType>           ParserFuncMapType;
  typedef std::map<std::string, ParserFuncType>::iterator ParserFuncMapIterator;

  Parser(const std::string& filename);
  ~Parser();

  void parse(Config& config);

 private:
  // ParserFuncMapType server_parsing_map_;
  // ParserFuncMapType location_parsing_map_;
  // ParserFuncMapType common_parsing_map_;
  TokensType tokens_;
};

#endif  // PARSER_HPP_
