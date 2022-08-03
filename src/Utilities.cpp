#include "Utilities.hpp"

namespace ft {

unsigned int hexStringToInt(const std::string& hex_string) {
  std::stringstream ss(hex_string);
  unsigned int      value;

  ss >> std::hex >> value;

  return value;
}

std::string strBidirectionalTrim(std::string& str, const std::string& charset) {
  size_t left_edge = str.find_first_not_of(charset);
  size_t right_edge = str.find_last_not_of(charset);

  if (left_edge == std::string::npos)
    return "";

  return str.substr(left_edge, right_edge - left_edge + 1);
}

std::string getUntilDelimiter(const std::string& str,
                              const std::string& delimiter, size_t& pos) {
  std::string line;
  size_t      new_pos;

  if ((new_pos = str.find(delimiter, pos)) != std::string::npos) {
    line = str.substr(pos, new_pos - pos);
    // O(1);
    pos = new_pos + delimiter.length();
  }

  return line;
}

std::string splitUntilDelimiter(std::string& str, std::string delimiter) {
  size_t      pos;
  std::string line;

  if ((pos = str.find(delimiter)) != std::string::npos) {
    line = str.substr(0, pos);
    // O(n), n is the length of new str.
    str.erase(0, pos + delimiter.length());
  }

  return line;
}

std::vector<std::string> splitByString(std::string str, std::string delimiter) {
  std::vector<std::string> str_vector;
  size_t                   pos;

  while ((pos = str.find(delimiter)) != std::string::npos) {
    str_vector.push_back(str.substr(0, pos));
    str.erase(0, pos + delimiter.length());
  }

  return str_vector;
}

}  // namespace ft
