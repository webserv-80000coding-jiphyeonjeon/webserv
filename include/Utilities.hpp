#ifndef WEBSERV_UTILITIES_HPP_
#define WEBSERV_UTILITIES_HPP_

#include <sstream>

#define MAXIMUM_URI_LIMIT 2048
#define MAXIMUM_PAYLOAD_LIMIT 8192

namespace ft {

template <typename T>
std::string toString(T value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

void        strLeftTrim(std::string& str, const std::string& charset);
void        strRightTrim(std::string& str, const std::string& charset);
std::string strBidirectionalTrim(std::string& str, const std::string& charset);
std::string getUntilDelimiter(const std::string& str,
                              const std::string delimiter, size_t& pos);
std::string splitUntilDelimiter(std::string& str, std::string delimiter);
std::vector<std::string> splitByString(std::string str, std::string delimiter);

}  // namespace ft

#endif