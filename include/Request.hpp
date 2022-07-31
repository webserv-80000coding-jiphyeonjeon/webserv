#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include <map>
#include <string>

enum Method { kNone, kGet, kPost, kDelete };
enum TransferCoding { kDefault, kChunked };
enum STATE { kContinuous, kEnd };

class RequestHeader {
 public:
  typedef std::string                              HeaderKeyType;
  typedef std::string                              HeaderValueType;
  typedef std::map<HeaderKeyType, HeaderValueType> HeaderMapType;

  typedef int16_t PortType;
  typedef int     ContentLengthType;
  typedef int     ContentTypeType;

  typedef void (RequestHeader::*ParseFuncType)(Method          method,
                                               HeaderValueType value);
  typedef std::map<HeaderKeyType, ParseFuncType> ParseFuncMapType;

  RequestHeader();
  ~RequestHeader();

  const HeaderMapType&     getHeaders() const;
  const PortType&          getPort() const;
  const ContentLengthType& getContentLength() const;
  const ContentTypeType&   getContentType() const;
  const TransferCoding&    getTransferCoding() const;

  void setHeaders(const HeaderMapType& headers);
  void setPort(const PortType& port);
  void setContentLength(const ContentLengthType& contentLength);
  void setContentType(const ContentTypeType& contentType);
  void setTransferCoding(const TransferCoding& transferCoding);

  void parseHost(Method method, HeaderValueType value);
  void parseContentLength(Method method, HeaderValueType value);
  void parseContentType(Method method, HeaderValueType value);
  void parseTransferCoding(Method method, HeaderValueType value);

 private:
  HeaderMapType     headers_;
  PortType          port_;
  ContentLengthType content_length_;
  ContentTypeType   content_type_;
  TransferCoding    transfer_coding_;
};

class Request {
 public:
 private:
};

#endif