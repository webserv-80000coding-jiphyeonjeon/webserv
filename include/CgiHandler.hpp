#ifndef CGIHANDLER_HPP_
#define CGIHANDLER_HPP_

#include <string>

#include "Request.hpp"
#include "Response.hpp"

class CgiHandler {
 public:
  typedef std::string              CgiPathType;
  typedef std::string              CgiResponseType;
  typedef Response::BodyType       BodyType;
  typedef Response::StatusCodeType StatusCodeType;
  typedef Request::HeaderKeyType   HeaderKeyType;
  typedef Request::HeaderValueType HeaderValueType;
  typedef Request::HeaderMapType   HeaderMapType;

  CgiHandler(const CgiPathType& cgi_path, const Request& request);
  ~CgiHandler();

  const StatusCodeType& getStatusCode() const;
  const HeaderMapType&  getHeaderMap() const;
  const BodyType&       getBody() const;

  void cgiExecute();

 private:
  static const int kCgiBufferSize = 65536;

  void parseCgiResponse(CgiResponseType& cgi_response);

  int initEnviron(const Request& request);
  int initPipe();

  int                fpipe_[2];
  int                bpipe_[2];
  const CgiPathType& cgi_path_;

  StatusCodeType status_code_;
  HeaderMapType  header_map_;
  BodyType       body_;
};

#endif  // CGIHANDLER_HPP_
