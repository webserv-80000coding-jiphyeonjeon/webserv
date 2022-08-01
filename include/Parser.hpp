// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <arpa/inet.h>

#include "Config.hpp"
#include "Scanner.hpp"

enum ResultConstant { kOk = 0, kError = -1, kUnset = -2 };
enum FunctionType { kServer, kLocation, kCommon };

class Parser {
 public:
  typedef Scanner::TokensType TokensType;

  typedef ConfigServer::AddressType AddressType;
  typedef ConfigServer::PortType    PortType;

  typedef void (Parser::*ServerParserFuncType)(ConfigServer&,
                                               const TokensType&);
  typedef std::map<std::string, ServerParserFuncType> ServerParserFuncMapType;
  typedef ServerParserFuncMapType::iterator ServerParserFuncMapIterator;

  typedef void (Parser::*LocationParserFuncType)(ConfigLocation&,
                                                 const TokensType&);
  typedef std::map<std::string, LocationParserFuncType>
                                              LocationParserFuncMapType;
  typedef LocationParserFuncMapType::iterator LocationParserFuncMapIterator;

  typedef void (Parser::*CommonParserFuncType)(ConfigCommon&,
                                               const TokensType&);
  typedef std::map<std::string, CommonParserFuncType> CommonParserFuncMapType;
  typedef CommonParserFuncMapType::iterator CommonParserFuncMapIterator;

  typedef ResultConstant ResultType;

  Parser(const std::string& filename);
  ~Parser();

  void parse(Config& config);

 private:
  static const AddressType kDefaultAddress = INADDR_ANY;
  static const PortType    kDefaultPort = 4242;
  static const AddressType kLocalhost = 2130706433;

  ServerParserFuncMapType   server_parsing_map_;
  LocationParserFuncMapType location_parsing_map_;
  CommonParserFuncMapType   common_parsing_map_;
  TokensType                tokens_;

  void initServerParsingMap();
  void initLocationParsingMap();
  void initCommonParsingMap();

  void parseServer(ConfigServer& server, size_t& idx);
  void parseLocation(ConfigLocation& location, size_t& idx);
  void fillDefaultConfigServer(ConfigServer& server);
  void fillDefaultConfigLocation(ConfigServer&               server,
                                 ConfigServer::LocationType& locations);

  void parseServerName(ConfigServer& server, const TokensType& args);
  void parseListen(ConfigServer& server, const TokensType& args);

  void parseLimitExcept(ConfigLocation& location, const TokensType& args);

  void parseAutoindex(ConfigCommon& common, const TokensType& args);
  void parseClientBodyBufferSize(ConfigCommon& common, const TokensType& args);
  void parseErrorPage(ConfigCommon& common, const TokensType& args);
  void parseIndex(ConfigCommon& common, const TokensType& args);
  void parseRoot(ConfigCommon& common, const TokensType& args);

  bool isServerDirective(ServerParserFuncMapIterator& it_server,
                         CommonParserFuncMapIterator& it_common,
                         const std::string&           token);

  bool isLocationDirective(LocationParserFuncMapIterator& it_location,
                           CommonParserFuncMapIterator&   it_common,
                           const std::string&             token);

  void serverParserFunctionCall(ConfigServer& server, ConfigCommon& common,
                                TokensType& args, const std::string& directive,
                                const FunctionType type);

  void locationParserFunctionCall(ConfigLocation& location,
                                  ConfigCommon& common, TokensType& args,
                                  const std::string& directive,
                                  const FunctionType type);
};

#endif  // PARSER_HPP_
