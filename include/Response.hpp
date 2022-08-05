#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include <map>
#include <sstream>
#include <string>

class Response {
 public:
  typedef std::string                              MessageType;
  typedef std::string                              VersionType;
  typedef int                                      StatusCodeType;
  typedef std::string                              StatusCodeMessageType;
  typedef std::map<int, std::string>               StatusCodeMapType;
  typedef std::string                              HeaderKeyType;
  typedef std::string                              HeaderValueType;
  typedef std::map<HeaderKeyType, HeaderValueType> HeaderMapType;
  typedef std::string                              BodyType;

  Response();
  ~Response();

  const MessageType& getMessage() const;

  void setVersion(const VersionType& version);
  void setStatusCode(const StatusCodeType& status_code);
  void setHeader(const HeaderKeyType& key, const HeaderValueType& value);
  void setBody(const BodyType& body);

  void build();
  void buildException(const StatusCodeType& status_code);

  bool isBuilt() const;

 private:
  void initStatusCodeMap();

  MessageType       message_;
  VersionType       version_;
  StatusCodeType    status_code_;
  StatusCodeMapType status_code_map_;
  HeaderMapType     header_map_;
  BodyType          body_;
};

#endif
