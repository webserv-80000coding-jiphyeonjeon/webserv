// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "Config.hpp"
#include "Scanner.hpp"

enum ResultConstant { kOk = 0, kError = -1, kUnset = -2 };

class Parser {
 public:
  typedef Scanner::TokensType TokensType;

  typedef void (Parser::*ServerParserFuncType)(ConfigServer&, const TokensType&);
  typedef std::map<std::string, ServerParserFuncType> ServerParserFuncMapType;
  typedef ServerParserFuncMapType::iterator           ServerParserFuncMapIterator;

  // typedef void (Parser::*LocationParserFuncType)(ConfigLocation&, const TokensType&);
  // typedef std::map<std::string, LocationParserFuncType> LocationParserFuncMapType;
  // typedef LocationParserFuncMapType::iterator           LocationParserFuncMapIterator;

  // typedef void (Parser::*CommonParserFuncType)(ConfigCommon&, const TokensType&);
  // typedef std::map<std::string, CommonParserFuncType> CommonParserFuncMapType;
  // typedef CommonParserFuncMapType::iterator           CommonParserFuncMapIterator;

  typedef ResultConstant ResultType;

  Parser(const std::string& filename);
  ~Parser();

  void parse(Config& config);

 private:
  ServerParserFuncMapType server_parsing_map_;
  // LocationParserFuncMapType location_parsing_map_;
  // CommonParserFuncMapType   common_parsing_map_;
  TokensType tokens_;

  void initServerParsingMap();
  // void initLocationParsingMap();
  // void initCommonParsingMap();

  ResultType parseServer(ConfigServer& server, size_t& idx);

  void parseServerName(ConfigServer& server, const TokensType& args);
  void parseListen(ConfigServer& server, const TokensType& args);
};

#endif  // PARSER_HPP_
