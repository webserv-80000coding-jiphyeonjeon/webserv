#ifndef WEBSERV_PRACTICE_REQUEST_HEADER_HPP_
#define WEBSERV_PRACTICE_REQUEST_HEADER_HPP_

#include <map>
#include <string>

enum Method { kNone, kGet, kPost, kDelete };
enum TransferCoding { kDefault, kChunked };
enum STATE { kContinuous, kEnd };

class RequestHeader {
 public:
  typedef std::map<std::string, std::string> HeaderMap;
  typedef int                                Flag;
  typedef void (RequestHeader::*HeaderParseFunc)(Flag        method,
                                                 std::string value);
  typedef std::map<std::string, HeaderParseFunc> HeaderParseFuncMap;

  RequestHeader();
  ~RequestHeader();

  const HeaderMap   &getHeaders();
  const std::string &getHeader(std::string key);
  const int         &getHostPort();
  const int         &getContentLength();
  const Flag        &getContentType();
  const Flag        &getTransferEncoding();

  void setHeaders(HeaderMap headers);
  void setHeader(std::string key, std::string value, Flag method);
  void setHostPort(int host_port);
  void setContentLength(int content_length);
  void setContentType(Flag content_type);
  void setTransferEncoding(Flag transfer_encoding);

  void turnOnContentTypeFlag(int flag);
  void turnOnTransferEncodingFlag(int flag);

  void parseHost(Flag method, std::string value);
  void parseContentLength(Flag method, std::string value);
  void parseContentType(Flag method, std::string value);
  void parseTransferEncoding(Flag method, std::string value);

  void printRequestHeader();

 private:
  HeaderParseFuncMap headerParseFuncMap_;

  HeaderParseFuncMap createHeaderParseFuncMap();

  HeaderMap headers_;
  int       host_port_;
  int       content_length_;
  Flag      content_type_;
  Flag      transfer_coding_;
};

#endif  //
