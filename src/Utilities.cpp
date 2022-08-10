#include "Utilities.hpp"

#include <fstream>

namespace ft {

unsigned int hexStringToInt(const std::string& hex_string) {
  std::stringstream ss(hex_string);
  unsigned int      value;

  ss >> std::hex >> value;

  return value;
}

std::string strBidirectionalTrim(std::string& str, const std::string& charset) {
  size_t left_edge = str.find_first_not_of(charset);
  size_t right_edge = str.find_last_not_of(charset);

  if (left_edge == std::string::npos)
    return "";

  return str.substr(left_edge, right_edge - left_edge + 1);
}

std::string getUntilDelimiter(const std::string& str,
                              const std::string& delimiter, size_t& pos) {
  std::string line;
  size_t      new_pos;

  if ((new_pos = str.find(delimiter, pos)) != std::string::npos) {
    line = str.substr(pos, new_pos - pos);
    // O(1);
    pos = new_pos + delimiter.length();
  }

  return line;
}

std::string splitUntilDelimiter(std::string& str, std::string delimiter) {
  size_t      pos;
  std::string line;

  if ((pos = str.find(delimiter)) != std::string::npos) {
    line = str.substr(0, pos);
    // O(n), n is the length of new str.
    str.erase(0, pos + delimiter.length());
  }

  return line;
}

std::vector<std::string> splitByString(std::string str, std::string delimiter) {
  std::vector<std::string> str_vector;
  size_t                   pos;

  while ((pos = str.find(delimiter)) != std::string::npos) {
    str_vector.push_back(str.substr(0, pos));
    str.erase(0, pos + delimiter.length());
  }

  return str_vector;
}

bool isDigits(const std::string& str) {
  return str.find_first_not_of("0123456789") == std::string::npos;
}

MIMEMapType initMIMEMap() {
  MIMEMapType mime_map;

  mime_map[".aac"] = "audio/aac";
  mime_map[".abw"] = "application/x-abiword";
  mime_map[".arc"] = "application/octet-stream";
  mime_map[".avi"] = "video/x-msvideo";
  mime_map[".azw"] = "application/vnd.amazon.ebook";
  mime_map[".bin"] = "application/octet-stream";
  mime_map[".bz"] = "application/x-bzip";
  mime_map[".bz2"] = "application/x-bzip2";
  mime_map[".csh"] = "application/x-csh";
  mime_map[".css"] = "text/css";
  mime_map[".csv"] = "text/csv";
  mime_map[".doc"] = "application/msword";
  mime_map[".epub"] = "application/epub+zip";
  mime_map[".gif"] = "image/gif";
  mime_map[".htm"] = "text/html";
  mime_map[".html"] = "text/html";
  mime_map[".ico"] = "image/x-icon";
  mime_map[".ics"] = "text/calendar";
  mime_map[".jar"] = "application/java-archive";
  mime_map[".jpeg"] = "image/jpeg";
  mime_map[".jpg"] = "image/jpeg";
  mime_map[".js"] = "text/javascript";
  mime_map[".json"] = "application/json";
  mime_map[".mid"] = "audio/midi";
  mime_map[".midi"] = "audio/midi";
  mime_map[".mpeg"] = "video/mpeg";
  mime_map[".mpkg"] = "application/vnd.apple.installer+xml";
  mime_map[".odp"] = "application/vnd.oasis.opendocument.presentation";
  mime_map[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
  mime_map[".odt"] = "application/vnd.oasis.opendocument.text";
  mime_map[".oga"] = "audio/ogg";
  mime_map[".ogv"] = "video/ogg";
  mime_map[".ogx"] = "application/ogg";
  mime_map[".pdf"] = "application/pdf";
  mime_map[".ppt"] = "application/vnd.ms-powerpoint";
  mime_map[".rar"] = "application/x-rar-compressed";
  mime_map[".rtf"] = "application/rtf";
  mime_map[".sh"] = "application/x-sh";
  mime_map[".svg"] = "image/svg+xml";
  mime_map[".swf"] = "application/x-shockwave-flash";
  mime_map[".tar"] = "application/x-tar";
  mime_map[".tif"] = "image/tiff";
  mime_map[".tiff"] = "image/tiff";
  mime_map[".ttf"] = "application/x-font-ttf";
  mime_map[".vsd"] = "application/vnd.visio";
  mime_map[".wav"] = "audio/x-wav";
  mime_map[".weba"] = "audio/webm";
  mime_map[".webm"] = "video/webm";
  mime_map[".webp"] = "image/webp";
  mime_map[".woff"] = "application/x-font-woff";
  mime_map[".xhtml"] = "application/xhtml+xml";
  mime_map[".xls"] = "application/vnd.ms-excel";
  mime_map[".xml"] = "application/xml";
  mime_map[".xul"] = "application/vnd.mozilla.xul+xml";
  mime_map[".zip"] = "application/zip";
  mime_map[".3gp"] = "video/3gpp";
  mime_map[".3g2"] = "video/3gpp2";
  mime_map[".7z"] = "application/x-7z-compressed";
  mime_map[".mp3"] = "audio/mpeg";
  mime_map[".mp4"] = "video/mp4";
  mime_map[".mpeg"] = "video/mpeg";
  mime_map[".mpg"] = "video/mpeg";
  mime_map[".mov"] = "video/quicktime";
  mime_map[".webm"] = "video/webm";
  mime_map[""] = "text/plain";
  mime_map[".bin"] = "application/octet-stream";

  return mime_map;
}

MIMEMapType mime_map_ = initMIMEMap();

const MIMEType& getMIME(const FileManager& file_manager) {
  std::string extension = file_manager.getExtension();
  // check is binary(octet-stream) or text/plain
  if (extension == "" || extension == ".") {
    int           c;
    std::ifstream a(file_manager.getFullName());
    while ((c = a.get()) != EOF && c <= 127)
      ;
    if (c == EOF)
      return mime_map_[""];
    return mime_map_[".bin"];
  }
  // if extension is not in mime_map_, return application/octet-stream
  if (mime_map_.find(extension) == mime_map_.end())
    return mime_map_[".bin"];
  return mime_map_[extension];
}

}  // namespace ft

ServerException::ServerException(const std::string&    message,
                                 const StatusCodeType& status_code)
    : message_(message), status_code_(status_code) {}

ServerException::~ServerException() throw() {}

const char* ServerException::what() const throw() { return message_.c_str(); }

const StatusCodeType& ServerException::getStatusCode() const {
  return status_code_;
}
