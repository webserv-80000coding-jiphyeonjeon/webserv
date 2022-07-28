#ifndef WEBSERV_PRACTICE_RESPONSE_HPP_
#define WEBSERV_PRACTICE_RESPONSE_HPP_

#include <map>
#include <string>

class Response {
 public:
  typedef std::map<std::string, std::string> HeaderMap;

  Response();
  ~Response();

  const std::string& getResponseMessage() const;
  const std::string& getVersion() const;
  const int&         getStatusCode() const;
  const HeaderMap&   getHeaderMap() const;
  const std::string& getHeader(const std::string& key) const;
  const std::string& getBody() const;

  void setVersion(std::string version);
  void setStatusCode(int status_code);
  void setHeaderMap(HeaderMap header_map);
  void setHeader(const std::string& key, const std::string& value);
  void setBody(std::string body);

  void build();
  void buildStartLine();
  void buildHeader();
  void buildBody();

  void print();

 private:
  static const std::map<int, std::string> kStatusCodes;

  static std::map<int, std::string> createStatusCodes();

  std::string response_message_;
  std::string version_;
  int         status_code_;
  HeaderMap   headerMap_;
  std::string body_;
};

#endif