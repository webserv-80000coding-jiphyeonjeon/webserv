#ifndef CGIHANDLER_HPP_
#define CGIHANDLER_HPP_

#include <string>

#include "Request.hpp"
#include "Response.hpp"

class CgiHandler {
 public:
  typedef std::string        CgiPathType;
  typedef Response::BodyType BodyType;

  CgiHandler(const CgiPathType& cgi_path, const Request& request);
  ~CgiHandler();

  const BodyType& getBody() const;

  void cgiExecute();

 private:
  static const int kCgiBufferSize = 65536;

  int initEnviron(const Request& request);
  int initPipe();

  int                fpipe_[2];
  int                bpipe_[2];
  const CgiPathType& cgi_path_;
  BodyType           body_;
};

#endif  // CGIHANDLER_HPP_
