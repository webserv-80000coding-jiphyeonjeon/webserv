#include <iostream>
#include <string>
#include <vector>

namespace ft {

std::vector<std::string> split_by_string(std::string str, std::string delimiter) {
  std::vector<std::string> str_vector;
  size_t                   pos;

  while ((pos = str.find(delimiter)) != std::string::npos) {
    str_vector.push_back(str.substr(0, pos));
    std::cout << str_vector.back() << std::endl;
    std::cout << "======" << std::endl;
    str.erase(0, pos + delimiter.length());
  }

  return str_vector;
}

}  // namespace ft