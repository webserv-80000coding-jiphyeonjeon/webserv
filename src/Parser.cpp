#include "Parser.hpp"

Parser::Parser(const std::string& filename) : tokens_() { Scanner::readFile(tokens_, filename); }

Parser::~Parser() {}

void Parser::parse(Config& config) { config.setServers(tokens_); }
