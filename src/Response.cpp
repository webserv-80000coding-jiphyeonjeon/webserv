#include "Response.hpp"

#include "Utilities.hpp"

Response::Response() : version_("HTTP/1.1"), status_code_(200) {
  initStatusCodeMap();
}

Response::~Response() {}

const Response::MessageType& Response::getMessage() const { return message_; }

void Response::setVersion(const VersionType& version) { version_ = version; }

void Response::setStatusCode(const StatusCodeType& status_code) {
  status_code_ = status_code;
}

void Response::setHeader(const HeaderKeyType&   key,
                         const HeaderValueType& value) {
  header_map_[key] = value;
}

void Response::setBody(const BodyType& body) {
  body_ = body;
  setHeader("Content-Length", ft::toString(body_.size()));
}

void Response::build() {
  if (isBuilt())
    return;

  std::stringstream ss;
  ss << version_ << " " << status_code_ << " " << status_code_map_[status_code_]
     << "\r\n";
  for (HeaderMapType::const_iterator it = header_map_.begin();
       it != header_map_.end(); ++it)
    ss << (*it).first << ": " << (*it).second << "\r\n";
  ss << "\r\n";
  ss << body_;
  message_ = ss.str();
}

void Response::buildException(const StatusCodeType& status_code) {
  std::stringstream ss;
  setStatusCode(status_code);
  ss << "<html>\n<head><title>" << status_code_ << " "
     << status_code_map_[status_code_]
     << "</title></head>\n<body>\n<center><h1>" << status_code_ << " "
     << status_code_map_[status_code_]
     << "</h1></center>\n<hr><center>80000coding-jiphyeonjeon</center>\n</"
        "body>\n</html>\n";
  setHeader("Content-Type", "text/html");
  setBody(ss.str());
  build();
}

bool Response::isBuilt() const { return message_ != ""; }

void Response::initStatusCodeMap() {
  status_code_map_[100] = "Continue";
  status_code_map_[101] = "Switching Protocols";
  status_code_map_[102] = "Processing";
  status_code_map_[200] = "OK";
  status_code_map_[201] = "Created";
  status_code_map_[202] = "Accepted";
  status_code_map_[203] = "Non-Authoritative Information";
  status_code_map_[204] = "No Content";
  status_code_map_[205] = "Reset Content";
  status_code_map_[206] = "Partial Content";
  status_code_map_[207] = "Multi-Status";
  status_code_map_[208] = "Already Reported";
  status_code_map_[226] = "IM Used";
  status_code_map_[300] = "Multiple Choices";
  status_code_map_[301] = "Moved Permanently";
  status_code_map_[302] = "Found";
  status_code_map_[303] = "See Other";
  status_code_map_[304] = "Not Modified";
  status_code_map_[305] = "Use Proxy";
  status_code_map_[306] = "Switch Proxy";
  status_code_map_[307] = "Temporary Redirect";
  status_code_map_[308] = "Permanent Redirect";
  status_code_map_[400] = "Bad Request";
  status_code_map_[401] = "Unauthorized";
  status_code_map_[402] = "Payment Required";
  status_code_map_[403] = "Forbidden";
  status_code_map_[404] = "Not Found";
  status_code_map_[405] = "Method Not Allowed";
  status_code_map_[406] = "Not Acceptable";
  status_code_map_[407] = "Proxy Authentication Required";
  status_code_map_[408] = "Request Timeout";
  status_code_map_[409] = "Conflict";
  status_code_map_[410] = "Gone";
  status_code_map_[411] = "Length Required";
  status_code_map_[412] = "Precondition Failed";
  status_code_map_[413] = "Request Entity Too Large";
  status_code_map_[414] = "Request-URI Too Long";
  status_code_map_[415] = "Unsupported Media Type";
  status_code_map_[416] = "Requested Range Not Satisfiable";
  status_code_map_[417] = "Expectation Failed";
  status_code_map_[500] = "Internal Server Error";
  status_code_map_[501] = "Not Implemented";
  status_code_map_[502] = "Bad Gateway";
  status_code_map_[503] = "Service Unavailable";
  status_code_map_[504] = "Gateway Timeout";
  status_code_map_[505] = "HTTP Version Not Supported";
  status_code_map_[506] = "Variant Also Negotiates";
  status_code_map_[507] = "Insufficient Storage";
  status_code_map_[508] = "Loop Detected";
  status_code_map_[509] = "Bandwidth Limit Exceeded";
  status_code_map_[510] = "Not Extended";
  status_code_map_[511] = "Network Authentication Required";
}
