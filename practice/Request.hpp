#ifndef WEBSERV_PRACTICE_REQUEST_HPP_
#define WEBSERV_PRACTICE_REQUEST_HPP_

#include <map>
#include <string>

enum Method { kNone, kGet, kPost, kDelete };
enum TransferCoding { kDefault, kChunked };
enum STATE { kContinuous, kEnd };

class Request {
 private:
  typedef int Flag;

  std::string                        request_message_;
  std::string                        method_;
  std::string                        path_;
  std::string                        version_;
  std::map<std::string, std::string> headers_;
  std::string                        body_;
  Flag                               state_;
  Flag                               header_flag_;
  Flag                               transfer_flag_;

 public:
  Request();
  ~Request();
  std::string                        getMethod();
  std::string                        getPath();
  std::string                        getVersion();
  std::string                        getHeader(std::string key);
  std::string                        getBody();
  std::map<std::string, std::string> getHeaders();
  Flag                               getState();
  Flag                               getHeaderFlag();
  Flag                               getTransferFlag();
  void                               setMethod(std::string method);
  void                               setPath(std::string path);
  void                               setVersion(std::string version);
  void                               setHeader(std::string key, std::string value);
  void                               setBody(std::string body);

  void parse(std::string request_message);
  void print();
};

#endif