#include "Scanner.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "color.hpp"

void Scanner::readFile(Scanner::TokensType& tokens,
                       const std::string&   filename) {
  if (filename.rfind(".conf") != filename.size() - 5)
    throw std::invalid_argument("Config: File name must end with '.conf'");

  std::ifstream file(filename);
  if (file.fail() || file.bad())
    throw std::invalid_argument("Config: Could not open file");

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  Scanner::tokenize(tokens, buffer.str());
}

void Scanner::tokenize(Scanner::TokensType& tokens, const std::string& str) {
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
    checkMetaCharacter(tokens, token);
    start = str.find_first_not_of(" \t\n", end);
  }
}

// TODO: meta character "{};" 앞뒤 공백이 없을 때도 구분할 수 있는 기능 추가
// 필요 현재는 문자열 맨 뒤에 ;가 붙어있는 경우만 처리
void Scanner::checkMetaCharacter(Scanner::TokensType& tokens,
                                 const std::string&   str) {
  if (str.rfind(";") == str.size() - 1) {
    std::string token = str.substr(0, str.size() - 1);
    if (token != "")
      tokens.push_back(token);
    tokens.push_back(";");
  } else
    tokens.push_back(str);
}

std::ostream& operator<<(std::ostream& os, const Scanner::TokensType& tokens) {
  char delimiter[2] = {'\0', '\0'};

  for (Scanner::TokensType::const_iterator it = tokens.begin();
       it != tokens.end(); ++it) {
    os << delimiter << *it;
    if (*it == "{" || *it == "}" || *it == ";") {
      delimiter[0] = '\n';
    } else {
      delimiter[0] = ' ';
    }
  }

  return os;
}
