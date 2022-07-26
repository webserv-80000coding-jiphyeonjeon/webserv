#include "Parser.hpp"

#include <iostream>

#include "color.hpp"

Parser::Parser(const std::string& filename) : tokens_() {
  Scanner::readFile(tokens_, filename);
  initServerParsingMap();
  initLocationParsingMap();
  initCommonParsingMap();
}

Parser::~Parser() {}

void Parser::parse(Config& config) {
  std::cout << tokens_ << std::endl;

  ConfigServer server;
  for (size_t i = 0; i < tokens_.size(); ++i) {
    if (tokens_[i] == "server") {
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
  server_parsing_map_["server_name"] = true;
  server_parsing_map_["listen"] = true;
}

void Parser::initLocationParsingMap() { location_parsing_map_["limit_except"] = true; }

void Parser::initCommonParsingMap() {
  common_parsing_map_["autoindex"] = true;
  common_parsing_map_["client_body_buffer_size"] = true;
  common_parsing_map_["error_page"] = true;
  common_parsing_map_["index"] = true;
  common_parsing_map_["root"] = true;
}

Parser::ResultType Parser::parseServer(ConfigServer& server, size_t& idx) {
  // TODO 일단은 전달할 인자값 확인만 하기
  (void)server;
  TokensType            args;
  ParserFuncMapIterator it;
  std::string           directive;

  std::cout << "\n" BLU "--- parseServer ---" END << std::endl;
  for (; idx < tokens_.size() && tokens_[idx] != "}"; ++idx) {
    std::cout << "token: " << tokens_[idx] << std::endl;

    // 현재는 server_name, listen만 처리
    // TODO 내부 directive들을 처리하는 부분 처리 -> it 체크용 함수 구현 필요
    if ((it = server_parsing_map_.find(tokens_[idx])) != server_parsing_map_.end()) {
      // set directive
      if (directive != "") {
        std::cout << YEL "directive: " END << directive << " " << args << std::endl;
        if (args.back() != ";")
          throw std::invalid_argument(RED "Config: Directive must end with ';'" END);
        args.clear();
      }
      directive = tokens_[idx];
    } else {
      // set args
      args.push_back(tokens_[idx]);
    }
  }

  if (directive != "") {
    std::cout << YEL "directive: " END << directive << " " << args << std::endl;
    if (args.back() != ";")
      throw std::invalid_argument(RED "Config: Directive must end with ';'" END);
    args.clear();
  }

  // }가 없는 경우는 error
  if (tokens_[idx] != "}") {
    return kError;
  }

  std::cout << BLU "------------------ " END << std::endl;
  return kOk;
}
