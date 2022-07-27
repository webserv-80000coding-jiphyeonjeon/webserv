#include "Parser.hpp"

#include <arpa/inet.h>

#include <iostream>

#include "color.hpp"

Parser::Parser(const std::string& filename) : tokens_() {
  Scanner::readFile(tokens_, filename);
  initServerParsingMap();
  // initLocationParsingMap();
  initCommonParsingMap();
}

Parser::~Parser() {}

void Parser::parse(Config& config) {
  std::cout << tokens_ << std::endl;

  for (size_t i = 0; i < tokens_.size(); ++i) {
    if (tokens_[i] == "server") {
      ConfigServer server;
      // validate server block
      if (tokens_[++i] != "{") {
        throw std::invalid_argument(RED "Config: Server block must start with '{'" END);
      }

      // parse server block
      // 만약 }가 없는 상태라면, 내부 파싱처리 중에 directive 오류가 될테니 따로 처리 X
      // TODO 처리하고 싶다면 tokenize할 때 미리 확인해야 할텐데....
      if (parseServer(server, ++i) == kError) {
        throw std::invalid_argument(RED "Config: Server block parsing error" END);
      }

      // push to config
      config.addServers(server);
    } else {
      throw std::invalid_argument(RED "Config: Server block is not found." END);
    }
  }
}

void Parser::initServerParsingMap() {
  server_parsing_map_["server_name"] = &Parser::parseServerName;
  server_parsing_map_["listen"] = &Parser::parseListen;
}

// void Parser::initLocationParsingMap() { location_parsing_map_["limit_except"] = true; }

void Parser::initCommonParsingMap() {
  common_parsing_map_["autoindex"] = &Parser::parseAutoindex;
  common_parsing_map_["client_body_buffer_size"] = &Parser::parseClientBodyBufferSize;
  common_parsing_map_["error_page"] = &Parser::parseErrorPage;
  common_parsing_map_["index"] = &Parser::parseIndex;
  common_parsing_map_["root"] = &Parser::parseRoot;
}

Parser::ResultType Parser::parseServer(ConfigServer& server, size_t& idx) {
  ConfigCommon   common;
  ConfigLocation location;

  ServerParserFuncMapIterator it_server;
  CommonParserFuncMapIterator it_common;
  FunctionType                type;
  std::string                 directive;
  TokensType                  args;

  std::cout << "\n" BLU "--- parseServer ---" END << std::endl;
  for (; idx < tokens_.size() && tokens_[idx] != "}"; ++idx) {
    std::cout << "token: " << tokens_[idx] << std::endl;

    // TODO 내부 directive들을 처리하는 부분 처리 -> it 체크용 함수 구현 필요
    if (((it_server = server_parsing_map_.find(tokens_[idx])) != server_parsing_map_.end()) ||
        ((it_common = common_parsing_map_.find(tokens_[idx])) != common_parsing_map_.end())) {
      // set directive
      if (directive != "") {
        std::cout << YEL "[" << directive << "] " END << args << std::endl;
        if (args.back() != ";")
          throw std::invalid_argument(RED "Config: Directive must end with ';'" END);
        if (type == kServer)
          (this->*server_parsing_map_[directive])(server, args);
        else
          (this->*common_parsing_map_[directive])(common, args);
        args.clear();
      }
      type = (it_server != server_parsing_map_.end()) ? kServer : kCommon;
      directive = tokens_[idx];
    } else if (tokens_[idx] == "location") {
      // TODO location parsing
    } else {
      // set args
      args.push_back(tokens_[idx]);
    }
  }

  if (directive != "") {
    std::cout << YEL "[" << directive << "] " END << args << std::endl;
    if (args.back() != ";")
      throw std::invalid_argument(RED "Config: Directive must end with ';'" END);
    if (type == kServer)
      (this->*server_parsing_map_[directive])(server, args);
    else
      (this->*common_parsing_map_[directive])(common, args);
    args.clear();
  }

  // }가 없는 경우는 error
  // TODO throw로 처리하는 것도 고민해보기
  if (tokens_[idx] != "}") {
    return kError;
  }

  std::cout << BLU "------------------ " END "\n" << std::endl;
  // parsing이 끝났으니 ConfigServer내부에 연결해주기
  server.setCommon(common);
  return kOk;
}

void Parser::parseServerName(ConfigServer& server, const Parser::TokensType& args) {
  // 만약 args.size() == 1이면 ;만 있다는 뜻이므로 오류
  if (args.size() == 1)
    throw std::invalid_argument(RED "Config: server_name: Invalid arguments" END
                                    "\nUsage: server_name name ...");

  // server name은 여러개가 올 수 있음
  for (size_t i = 0; i < args.size() - 1; ++i)
    server.addServerName(args[i]);
}

// TODO utils 함수 모으기
bool isDigits(const std::string& str) {
  return str.find_first_not_of("0123456789") == std::string::npos;
}

void Parser::parseListen(ConfigServer& server, const Parser::TokensType& args) {
  const ConfigServer::AddressType kLocalhost = 2130706433;
  const ConfigServer::AddressType kDefaultAddress = 0;
  const ConfigServer::PortType    kDefaultPort = 4242;

  ConfigServer::AddressType addr = kDefaultAddress;
  ConfigServer::PortType    port = kDefaultPort;
  size_t                    delimiter;

  if (args.size() != 2)
    throw std::invalid_argument(RED "Config: listen: Invalid arguments." END
                                    "\nUsage: listen address[:port] or listen port");

  // port만 있는 경우: address = 0(default)
  // address만 있는 경우 (.이 있거나 localhost): port = 4242(default)
  if ((delimiter = args[0].find(':')) == std::string::npos) {
    if (args[0] == "localhost") {
      addr = htonl(kLocalhost);
    } else if (args[0].find('.') != std::string::npos) {
      addr = inet_addr(args[0].c_str());
    } else {
      // TODO (uint16_t 범위에서 파싱될 수 있도록 함수 추가하기)
      if (!isDigits(args[0]))
        throw std::invalid_argument(RED "Config: listen: Invalid arguments." END
                                        "\nUsage: listen address[:port] or listen port");
      port = atoi(args[0].c_str());
    }
  } else {
    std::string addr_str = args[0].substr(0, delimiter);
    if (addr_str == "localhost")
      addr = htonl(kLocalhost);
    else if (addr_str != "*")
      addr = inet_addr(addr_str.c_str());

    std::string port_str = args[0].substr(delimiter + 1);
    if (!isDigits(port_str))
      throw std::invalid_argument(RED "Config: listen: Invalid arguments." END
                                      "\nUsage: listen address[:port] or listen port");
    port = atoi(port_str.c_str());
  }

  if (addr == INADDR_NONE)
    throw std::invalid_argument(RED "Config: listen: Invalid address (host)." END);
  server.addListen(std::make_pair(addr, port));
}

void Parser::parseAutoindex(ConfigCommon& common, const Parser::TokensType& args) {
  if (args.size() != 2 && args[0] != "on" && args[0] != "off")
    throw std::invalid_argument(RED "Config: autoindex: Invalid arguments." END
                                    "\nUsage: autoindex on | off");
  common.setAutoindex(args[0] == "on");
}

void Parser::parseClientBodyBufferSize(ConfigCommon& common, const Parser::TokensType& args) {
  if (args.size() != 2 && !isDigits(args[0]))
    throw std::invalid_argument(RED "Config: client_body_buffer_size: Invalid arguments." END
                                    "\nUsage: client_body_buffer_size size");
  common.setClientBodyBufferSize(atoi(args[0].c_str()));
}

void Parser::parseErrorPage(ConfigCommon& common, const Parser::TokensType& args) {
  if (args.size() < 3)
    throw std::invalid_argument(RED "Config: error_page: Invalid arguments." END
                                    "\nUsage: error_page code ... uri");

  size_t uri_pos = args.size() - 2;
  for (size_t i = 0; i < uri_pos; i++) {
    if (!isDigits(args[i]))
      throw std::invalid_argument(RED "Config: error_page: Invalid error code." END);
    common.addErrorPage(std::make_pair(atoi(args[i].c_str()), args[uri_pos]));
  }
}

void Parser::parseIndex(ConfigCommon& common, const Parser::TokensType& args) {
  if (args.size() == 1)
    throw std::invalid_argument(RED "Config: Invalid index arguments." END
                                    "\nUsage: index file ...");

  for (size_t i = 0; i < args.size() - 1; ++i)
    common.addIndex(args[i]);
}

void Parser::parseRoot(ConfigCommon& common, const Parser::TokensType& args) {
  if (args.size() != 2)
    throw std::invalid_argument(RED "Config: Invalid root arguments." END "\nUsage: root path");
  common.setRoot(args[0]);
}
