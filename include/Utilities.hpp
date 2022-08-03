#ifndef WEBSERV_UTILITIES_HPP_
#define WEBSERV_UTILITIES_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define MAXIMUM_URI_LIMIT 2048
#define MAXIMUM_PAYLOAD_LIMIT 8192

typedef unsigned short uint16_t;

namespace ft {
template <typename T>
std::string toString(T value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

unsigned int hexStringToInt(const std::string& hex_string);

void        strLeftTrim(std::string& str, const std::string& charset);
void        strRightTrim(std::string& str, const std::string& charset);
std::string strBidirectionalTrim(std::string& str, const std::string& charset);
std::string getUntilDelimiter(const std::string& str,
                              const std::string& delimiter, size_t& pos);
std::string splitUntilDelimiter(std::string& str, std::string delimiter);
std::vector<std::string> splitByString(std::string str, std::string delimiter);

}  // namespace ft

#endif
