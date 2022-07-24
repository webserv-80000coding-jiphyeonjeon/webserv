#ifndef WEBSERV_PRACTICE_REQUEST_HPP_
#define WEBSERV_PRACTICE_REQUEST_HPP_

class Request {
 private:
  typedef int Flag;

  std::string                        method;
  std::string                        path;
  std::string                        version;
  std::map<std::string, std::string> headers;
  std::string                        body;
  int                                status_code;
  Flag                               header_flag;
  Flag                               transfer_coding_flag;

 public:
  Request();
  ~Request();
  std::string                        getMethod();
  std::string                        getPath();
  std::string                        getVersion();
  std::string                        getHeader(std::string key);
  std::string                        getBody();
  std::vector<std::string>           getBodyLines();
  std::map<std::string, std::string> getHeaders();
  void                               setMethod(std::string method);
  void                               setPath(std::string path);
  void                               setVersion(std::string version);
  void                               setHeader(std::string key, std::string value);
  void                               setBody(std::string body);

  void print();
};

#endif