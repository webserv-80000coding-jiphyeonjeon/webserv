#ifndef WEBSERV_UTILITIES_HPP_
#define WEBSERV_UTILITIES_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define MAXIMUM_URI_LIMIT 2048
#define MAXIMUM_PAYLOAD_LIMIT 8192
#define FILE_READ_BUFFER_SIZE 8192

namespace ft {
template <typename T>
std::string toString(T value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

unsigned int hexStringToInt(const std::string& hex_string);

std::string strBidirectionalTrim(std::string& str, const std::string& charset);
std::string getUntilDelimiter(const std::string& str,
                              const std::string& delimiter, size_t& pos);
std::string splitUntilDelimiter(std::string& str, std::string delimiter);
std::vector<std::string> splitByString(std::string str, std::string delimiter);

bool isDigits(const std::string& str);

}  // namespace ft

typedef int StatusCodeType;

class ServerException : public std::exception {
 public:
  ServerException(const std::string&    message,
                  const StatusCodeType& status_code);
  virtual ~ServerException() throw();
  virtual const char*           what() const throw();
  virtual const StatusCodeType& getStatusCode() const;

 protected:
  std::string    message_;
  StatusCodeType status_code_;
};

#endif
