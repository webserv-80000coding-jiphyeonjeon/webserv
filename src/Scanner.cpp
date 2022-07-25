#include "Scanner.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "color.hpp"

Scanner::Scanner() {}
Scanner::~Scanner() {}

std::string Scanner::readFile(const std::string& filename) {
  if (filename.rfind(".conf") != filename.size() - 5)
    throw std::invalid_argument(RED "Config: File name must end with '.conf'" END);

  std::ifstream file(filename);
  if (file.fail() || file.bad())
    throw std::invalid_argument(RED "Config: Could not open file" END);

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  return buffer.str();
}
