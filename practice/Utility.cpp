#include <iostream>
#include <string>
#include <vector>

namespace ft {

void str_ltrim(std::string& str, const std::string& charset) {
  str.erase(0, str.find_first_not_of(charset));
}
void str_rtrim(std::string& str, const std::string& charset) {
  str.erase(str.find_last_not_of(charset) + 1);
}
std::string str_bidirectional_trim(std::string&       str,
                                   const std::string& charset) {
  size_t left_edge = str.find_first_not_of(charset);
  size_t right_edge = str.find_last_not_of(charset);

  return str.substr(left_edge, right_edge - left_edge + 1);
}

std::string get_until_delimiter(std::string& str, std::string delimiter,
                                size_t& pos) {
  std::string line;
  size_t      new_pos;

  if ((new_pos = str.find(delimiter, pos)) != std::string::npos) {
    line = str.substr(pos, new_pos - pos);
    // O(1);
    pos = new_pos + delimiter.length();
  }

  return line;
}

std::string split_until_delimiter(std::string& str, std::string delimiter) {
  size_t      pos;
  std::string line;

  if ((pos = str.find(delimiter)) != std::string::npos) {
    line = str.substr(0, pos);
    // O(n), n is the length of new str.
    str.erase(0, pos + delimiter.length());
  } else {
    line = std::string();
  }

  return line;
}

std::vector<std::string> split_by_string(std::string str,
                                         std::string delimiter) {
  std::vector<std::string> str_vector;
  size_t                   pos;

  while ((pos = str.find(delimiter)) != std::string::npos) {
    str_vector.push_back(str.substr(0, pos));
    str.erase(0, pos + delimiter.length());
  }

  return str_vector;
}

}  // namespace ft