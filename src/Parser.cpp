#include "Parser.hpp"

#include <iostream>

#include "Log.hpp"
#include "Utilities.hpp"
#include "color.hpp"

Parser::Parser(const std::string& filename) : tokens_() {
  ft::log.writeTimeLog("[Parser] --- Start to parsing ---");
  ft::log.writeTimeLog("Tokenizeing...");

  Scanner::readFile(tokens_, filename);
  ft::log.getLogStream() << tokens_ << std::endl;

  initServerParsingMap();
  initLocationParsingMap();
  initCommonParsingMap();
}

Parser::~Parser() {}

void Parser::parse(Config& config) {
  ft::log.writeTimeLog("Parsing...");

  for (size_t i = 0; i < tokens_.size(); ++i) {
    if (tokens_[i] == "server") {
      ConfigServer server;
      if (tokens_[++i] != "{")
        throw std::invalid_argument("Config: Server block must start with '{'");

      parseServer(server, ++i);
      config.addServers(server);

    } else {
      throw std::invalid_argument("Config: Server block is not found.");
    }
  }

  ft::log.writeTimeLog("[Parser] --- Success to config parsing ---");
}

void Parser::initServerParsingMap() {
  server_parsing_map_["server_name"] = &Parser::parseServerName;
  server_parsing_map_["listen"] = &Parser::parseListen;
}

void Parser::initLocationParsingMap() {
  location_parsing_map_["cgi"] = &Parser::parseCgi;
  location_parsing_map_["limit_except"] = &Parser::parseLimitExcept;
}

void Parser::initCommonParsingMap() {
  common_parsing_map_["autoindex"] = &Parser::parseAutoindex;
  common_parsing_map_["client_body_buffer_size"] =
      &Parser::parseClientBodyBufferSize;
  common_parsing_map_["error_page"] = &Parser::parseErrorPage;
  common_parsing_map_["index"] = &Parser::parseIndex;
  common_parsing_map_["return"] = &Parser::parseReturn;
  common_parsing_map_["root"] = &Parser::parseRoot;
}

void Parser::parseServer(ConfigServer& server, size_t& idx) {
  ft::log.writeLog(GRN "--- parseServer ---" END);

  ConfigServer::LocationType locations;
  ConfigCommon               common;

  ServerParserFuncMapIterator it_server;
  CommonParserFuncMapIterator it_common;
  FunctionType                type;
  std::string                 directive;
  TokensType                  args;

  for (; idx < tokens_.size() && tokens_[idx] != "}"; ++idx) {
    if (isServerDirective(it_server, it_common, tokens_[idx])) {
      if (directive != "")
        serverParserFunctionCall(server, common, args, directive, type);

      type = (it_server != server_parsing_map_.end()) ? kServer : kCommon;
      directive = tokens_[idx];

    } else if (tokens_[idx] == "location") {
      if (directive != "") {
        serverParserFunctionCall(server, common, args, directive, type);
        directive = "";
      }

      if (tokens_[++idx][0] != '/')
        throw std::invalid_argument("Config: Location uri must start with '/'");
      ConfigLocation location;
      std::string    location_uri = tokens_[idx];

      if (tokens_[++idx] != "{")
        throw std::invalid_argument(
            "Config: Location block must start with '{'");

      parseLocation(location, ++idx);
      locations.insert(std::make_pair(location_uri, location));

    } else {
      args.push_back(tokens_[idx]);
    }
  }

  if (directive != "")
    serverParserFunctionCall(server, common, args, directive, type);

  if (tokens_[idx] != "}")
    throw std::invalid_argument("Config: Server block must end with '}'");

  server.setCommon(common);
  fillDefaultConfigServer(server);
  fillDefaultConfigLocation(server, locations);
  server.setLocation(locations);

  ft::log.writeLog(GRN "-------------------" END);
}

void Parser::parseLocation(ConfigLocation& location, size_t& idx) {
  ft::log.writeLog(YEL "  --- parseLocation ---" END);

  ConfigCommon common;

  LocationParserFuncMapIterator it_location;
  CommonParserFuncMapIterator   it_common;
  FunctionType                  type;
  std::string                   directive;
  TokensType                    args;

  for (; idx < tokens_.size() && tokens_[idx] != "}"; ++idx) {
    if (isLocationDirective(it_location, it_common, tokens_[idx])) {
      if (directive != "")
        locationParserFunctionCall(location, common, args, directive, type);
      type = (it_location != location_parsing_map_.end()) ? kLocation : kCommon;
      directive = tokens_[idx];

    } else {
      args.push_back(tokens_[idx]);
    }
  }

  if (directive != "")
    locationParserFunctionCall(location, common, args, directive, type);

  if (tokens_[idx] != "}")
    throw std::invalid_argument("Config: Location block must end with '}'");

  location.setCommon(common);

  ft::log.writeLog(YEL "  --------------------- " END);
}

void Parser::fillDefaultConfigServer(ConfigServer& server) {
  if (server.getListen().empty())
    server.addListen(std::make_pair(INADDR_ANY, 4242));
  if (server.getIndex().empty())
    server.addIndex("index.html");
  if (server.getRoot() == "")
    server.setRoot("www");
}

void Parser::fillDefaultConfigLocation(ConfigServer&               server,
                                       ConfigServer::LocationType& locations) {
  if (locations.empty())
    locations.insert(std::make_pair("/", ConfigLocation()));

  for (ConfigServer::LocationType::iterator it = locations.begin();
       it != locations.end(); ++it) {
    if (it->second.getLimitExcept().empty()) {
      it->second.addLimitExcept("GET");
      it->second.addLimitExcept("HEAD");
      it->second.addLimitExcept("POST");
      it->second.addLimitExcept("PUT");
      it->second.addLimitExcept("DELETE");
    }

    if (it->second.getIndex().empty())
      it->second.setIndex(server.getIndex());
    if (it->second.getRoot() == "")
      it->second.setRoot(server.getRoot());
    if (it->second.getErrorPage().empty())
      it->second.setErrorPage(server.getErrorPage());
  }
}

void Parser::parseServerName(ConfigServer&             server,
                             const Parser::TokensType& args) {
  if (args.size() < 2)
    throw std::invalid_argument(
        "Config: server_name: Invalid arguments"
        "\nUsage: server_name name ...");

  for (size_t i = 0; i < args.size() - 1; ++i)
    server.addServerName(args[i]);
}

void Parser::parseListen(ConfigServer& server, const Parser::TokensType& args) {
  ConfigServer::AddressType addr = kDefaultAddress;
  ConfigServer::PortType    port = kDefaultPort;
  size_t                    delimiter;

  if (args.size() != 2)
    throw std::invalid_argument(
        "Config: listen: Invalid arguments."
        "\nUsage: listen address[:port] or listen port");

  if ((delimiter = args[0].find(':')) == std::string::npos) {
    if (args[0] == "localhost") {
      addr = htonl(kLocalhost);
    } else if (args[0].find('.') != std::string::npos) {
      addr = inet_addr(args[0].c_str());
    } else {
      if (!ft::isDigits(args[0]))
        throw std::invalid_argument(
            "Config: listen: Invalid arguments."
            "\nUsage: listen address[:port] or listen port");
      port = atoi(args[0].c_str());
    }

  } else {
    std::string addr_str = args[0].substr(0, delimiter);
    if (addr_str == "localhost") {
      addr = htonl(kLocalhost);
    } else if (addr_str != "*") {
      addr = inet_addr(addr_str.c_str());
    }

    std::string port_str = args[0].substr(delimiter + 1);
    if (!ft::isDigits(port_str))
      throw std::invalid_argument(
          "Config: listen: Invalid arguments."
          "\nUsage: listen address[:port] or listen port");
    port = atoi(port_str.c_str());
  }

  if (addr == INADDR_NONE)
    throw std::invalid_argument("Config: listen: Invalid address (host).");
  server.addListen(std::make_pair(addr, port));
}

void Parser::parseCgi(ConfigLocation&           location,
                      const Parser::TokensType& args) {
  if (args.size() != 3)
    throw std::invalid_argument(
        "Config: cgi: Invalid arguments."
        "\nUsage: cgi extenstion cgi_program_path");
  location.addCgi(std::make_pair(args[0], args[1]));
}

void Parser::parseLimitExcept(ConfigLocation&           location,
                              const Parser::TokensType& args) {
  if (args.size() < 2)
    throw std::invalid_argument(
        "Config: limit_except: Invalid arguments."
        "\nUsage: limit_except method ..."
        "\nmethod: GET, HEAD, POST, PUT, DELETE");

  for (size_t i = 0; i < args.size() - 1; ++i) {
    if (!location.isInMethodSet(args[i]))
      throw std::invalid_argument(
          "Config: limit_except: Invalid method."
          "\nmethod: GET, HEAD, POST, PUT, DELETE");
    location.addLimitExcept(args[i]);
  }
}

void Parser::parseAutoindex(ConfigCommon&             common,
                            const Parser::TokensType& args) {
  if (args.size() != 2 || (args[0] != "on" && args[0] != "off"))
    throw std::invalid_argument(
        "Config: autoindex: Invalid arguments."
        "\nUsage: autoindex on | off");
  common.setAutoindex(args[0] == "on");
}

void Parser::parseClientBodyBufferSize(ConfigCommon&             common,
                                       const Parser::TokensType& args) {
  if (args.size() != 2 || !ft::isDigits(args[0]))
    throw std::invalid_argument(
        "Config: client_body_buffer_size: Invalid arguments."
        "\nUsage: client_body_buffer_size size");
  common.setClientBodyBufferSize(atoi(args[0].c_str()));
}

void Parser::parseErrorPage(ConfigCommon&             common,
                            const Parser::TokensType& args) {
  if (args.size() < 3)
    throw std::invalid_argument(
        "Config: error_page: Invalid arguments."
        "\nUsage: error_page code ... uri");

  size_t uri_pos = args.size() - 2;
  for (size_t i = 0; i < uri_pos; i++) {
    if (!ft::isDigits(args[i]))
      throw std::invalid_argument("Config: error_page: Invalid error code.");
    common.addErrorPage(std::make_pair(atoi(args[i].c_str()), args[uri_pos]));
  }
}

void Parser::parseIndex(ConfigCommon& common, const Parser::TokensType& args) {
  if (args.size() < 2)
    throw std::invalid_argument(
        "Config: index: Invalid arguments."
        "\nUsage: index file ...");

  for (size_t i = 0; i < args.size() - 1; ++i)
    common.addIndex(args[i]);
}

void Parser::parseReturn(ConfigCommon& common, const Parser::TokensType& args) {
  if (args.size() != 3)
    throw std::invalid_argument(
        "Config: return: Invalid arguments."
        "\nUsage: return code url");

  if (!ft::isDigits(args[0]))
    throw std::invalid_argument("Config: return: Invalid code.");
  common.setReturn(std::make_pair(atoi(args[0].c_str()), args[1]));
}

void Parser::parseRoot(ConfigCommon& common, const Parser::TokensType& args) {
  if (args.size() != 2)
    throw std::invalid_argument(
        "Config: root: Invalid arguments."
        "\nUsage: root path");
  common.setRoot(args[0]);
}

bool Parser::isServerDirective(Parser::ServerParserFuncMapIterator& it_server,
                               Parser::CommonParserFuncMapIterator& it_common,
                               const std::string&                   token) {
  it_server = server_parsing_map_.find(token);
  it_common = common_parsing_map_.find(token);

  if ((it_server != server_parsing_map_.end()) ||
      (it_common != common_parsing_map_.end()))
    return true;
  return false;
}

bool Parser::isLocationDirective(
    Parser::LocationParserFuncMapIterator& it_location,
    Parser::CommonParserFuncMapIterator& it_common, const std::string& token) {
  it_location = location_parsing_map_.find(token);
  it_common = common_parsing_map_.find(token);

  if ((it_location != location_parsing_map_.end()) ||
      (it_common != common_parsing_map_.end()))
    return true;
  return false;
}

void Parser::serverParserFunctionCall(ConfigServer&       server,
                                      ConfigCommon&       common,
                                      Parser::TokensType& args,
                                      const std::string&  directive,
                                      const FunctionType  type) {
  ft::log.getLogStream() << GRN "[" << directive << "] " END << args
                         << std::endl;

  if (args.back() != ";")
    throw std::invalid_argument("Config: Directive must end with ';'");

  if (type == kServer) {
    (this->*server_parsing_map_[directive])(server, args);
  } else {
    (this->*common_parsing_map_[directive])(common, args);
  }
  args.clear();
}

void Parser::locationParserFunctionCall(ConfigLocation&     location,
                                        ConfigCommon&       common,
                                        Parser::TokensType& args,
                                        const std::string&  directive,
                                        const FunctionType  type) {
  ft::log.getLogStream() << YEL "  [" << directive << "] " END << args
                         << std::endl;

  if (args.back() != ";")
    throw std::invalid_argument("Config: Directive must end with ';'");

  if (type == kLocation) {
    (this->*location_parsing_map_[directive])(location, args);
  } else {
    (this->*common_parsing_map_[directive])(common, args);
  }
  args.clear();
}
