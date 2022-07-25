#include "Scanner.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "color.hpp"

Scanner::Scanner(const std::string& filename) : tokens_() { readFile(filename); }

const Scanner::TokensType& Scanner::getTokens() const { return tokens_; }

void Scanner::readFile(const std::string& filename) {
  if (filename.rfind(".conf") != filename.size() - 5)
    throw std::invalid_argument(RED "Config: File name must end with '.conf'" END);

  std::ifstream file(filename);
  if (file.fail() || file.bad())
    throw std::invalid_argument(RED "Config: Could not open file" END);

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  tokenize(buffer.str());
}

void Scanner::tokenize(const std::string& str) {
  std::size_t start = str.find_first_not_of(" \t\n");
  while (start != std::string::npos) {
    // remove comment
    if (str[start] == '#') {
      start = str.find_first_of("\n", start);
      if (start++ == std::string::npos)
        break;
      start = str.find_first_not_of(" \t\n", start);
      continue;
    }

    // push to tokens
    std::size_t end = str.find_first_of(" \t\n", start);
    if (end == std::string::npos)
      end = str.size();

    std::string token = str.substr(start, end - start);
    checkMetaCharacter(token);
    start = str.find_first_not_of(" \t\n", end);
  }
}

// TODO: meta character "{};" 앞뒤 공백이 없을 때도 구분할 수 있는 기능 추가 필요
// 현재는 문자열 맨 뒤에 ;가 붙어있는 경우만 처리
void Scanner::checkMetaCharacter(const std::string& str) {
  if (str.rfind(";") == str.size() - 1) {
    tokens_.push_back(str.substr(0, str.size() - 1));
    tokens_.push_back(";");
  } else
    tokens_.push_back(str);
}

std::ostream& operator<<(std::ostream& os, const Scanner::TokensType& tokens) {
  char delimiter[2] = {'\0', '\0'};

  for (std::vector<const std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
    os << delimiter << *it;
    if (*it == "{" || *it == "}" || *it == ";") {
      delimiter[0] = '\n';
    } else {
      delimiter[0] = ' ';
    }
  }

  return os;
}
