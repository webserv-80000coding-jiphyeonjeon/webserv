#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include <map>
#include <string>

enum Method { kNone, kGet, kPost, kPut, kDelete, kHead };
enum TransferCoding { kDefault, kChunked };
enum State { kContinuous, kEnd };
enum Level { kStartLine, kHeader, kBody, kDone };

// ================================================================
// RequestHeader
// ================================================================

class RequestHeader {
 public:
  typedef std::string                              HeaderKeyType;
  typedef std::string                              HeaderValueType;
  typedef std::map<HeaderKeyType, HeaderValueType> HeaderMapType;

  typedef std::string AddressType;
  typedef uint16_t    PortType;
  typedef int         ContentLengthType;
  typedef int         ContentTypeType;

  typedef void (RequestHeader::*ParseFuncType)(Method          method,
                                               HeaderValueType value);
  typedef std::map<HeaderKeyType, ParseFuncType> ParseFuncMapType;

  RequestHeader();
  ~RequestHeader();

  const HeaderMapType&     getHeaderMap() const;
  const HeaderValueType&   getHeader(const HeaderKeyType& key) const;
  const AddressType&       getAddress() const;
  const PortType&          getPort() const;
  const ContentLengthType& getContentLength() const;
  const ContentTypeType&   getContentType() const;
  const TransferCoding&    getTransferCoding() const;

  void setHeaderMap(const HeaderMapType& header_map, const Method& method);
  void setHeader(const HeaderKeyType& key, const HeaderValueType& value,
                 const Method& method);
  void setAddress(const AddressType& address);
  void setPort(const PortType& port);
  void setContentLength(const ContentLengthType& content_length);
  void setContentType(const ContentTypeType& content_type);
  void setTransferCoding(const TransferCoding& transfer_coding);

  void parseHost(Method method, HeaderValueType value);
  void parseContentLength(Method method, HeaderValueType value);
  void parseContentType(Method method, HeaderValueType value);
  void parseTransferCoding(Method method, HeaderValueType value);

  bool isChunked() const;

 private:
  ParseFuncMapType initParseFuncMap();

  HeaderMapType     header_map_;
  AddressType       address_;
  PortType          port_;
  ContentLengthType content_length_;
  ContentTypeType   content_type_;
  TransferCoding    transfer_coding_;

  ParseFuncMapType parse_func_map_;
};

// ================================================================
// Request
// ================================================================

class Request {
 public:
  typedef RequestHeader::HeaderMapType   HeaderMapType;
  typedef RequestHeader::HeaderKeyType   HeaderKeyType;
  typedef RequestHeader::HeaderValueType HeaderValueType;
  typedef std::string                    MessageType;
  typedef std::string                    PathType;
  typedef std::string                    VersionType;
  typedef std::string                    BodyType;
  typedef size_t                         PositionType;
  typedef int                            StatusCodeType;

  Request();
  ~Request();

  const MessageType&     getRequestMessage() const;
  const Method&          getMethod() const;
  const HeaderValueType& getHeader(const HeaderKeyType& key) const;
  const HeaderMapType&   getHeaderMap() const;
  const PathType&        getPath() const;
  const VersionType&     getVersion() const;
  const BodyType&        getBody() const;
  const State&           getState() const;

  void setRequestMessage(const MessageType& request_message);
  void setMethod(const Method& method);
  void setHeader(const HeaderKeyType& key, const HeaderValueType& value);
  void setPath(const PathType& path);
  void setVersion(const VersionType& version);
  void setBody(const BodyType& body);
  void setState(const State& state);

  int parse(MessageType& request_message);

  void print() const;

  class RequestException : public std::exception {
   public:
    RequestException(const std::string&    message,
                     const StatusCodeType& status_code)
        : message_(message), status_code_(status_code) {}
    virtual ~RequestException() throw() {}
    virtual const char*   what() const throw() { return message_.c_str(); }
    const StatusCodeType& getStatusCode() const { return status_code_; }

   private:
    std::string    message_;
    StatusCodeType status_code_;
  };

 private:
  void parseStartLine();
  void parseHeader();
  void parseBody();

  MessageType   request_message_;
  Method        method_;
  RequestHeader header_;
  PathType      path_;
  VersionType   version_;
  BodyType      body_;
  State         state_;
  Level         level_;
  PositionType  position_;
};

#endif