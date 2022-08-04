#include "File.hpp"

#include <sys/stat.h>

File::File() : fd_(-1) {}
File::File(const PathType& root, const PathType& path) : path_(path), fd_(-1) {
  parsePath(path);
}
File::File(const PathType& path) : fd_(-1) { parsePath(path); }
File::~File() { close(); }

const File::PathType&      File::getPath() const { return path_; }
const File::NameType&      File::getName() const { return name_; }
const File::ExtensionType& File::getExtension() const { return extension_; }
const File::FdType&        File::getFd() const { return fd_; }

void File::setPath(const PathType& path) { path_ = path; }
void File::setName(const NameType& name) { name_ = name; }
void File::setExtension(const ExtensionType& extension) {
  extension_ = extension;
}
void File::setFd(const FdType& fd) { fd_ = fd; }

bool File::open() {
  fd_ = ::open(path_.c_str(), O_RDWR);
  return fd_ == -1;
}

bool File::close() { return ::close(fd_); }

bool File::create() {
  fd_ = ::open(path_.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0755));
  return fd_ == -1;
}

bool File::remove() { return ::unlink(path_.c_str()); }

bool File::isExist() const {
  struct stat buf;
  return ::stat(path_.c_str(), &buf) == 0;
}

bool File::isDirectory() const {
  struct stat buf;
  return ::stat(path_.c_str(), &buf) == 0 && S_ISDIR(buf.st_mode);
}

bool File::isExist(const PathType& path) {
  struct stat buf;
  return ::stat(path.c_str(), &buf) == 0;
}

bool File::isDirectory(const PathType& path) {
  struct stat buf;
  return ::stat(path.c_str(), &buf) == 0 && S_ISDIR(buf.st_mode);
}

void parsePath(const PathType& path) {
  path_ = path;

  size_t name_pos = path_.find_last_of('/');
  size_t extension_pos = name_.find_last_of('.');

  if (name_pos == std::string::npos && extension_pos == std::string::npos) {
    name_ = path_;
  } else if (name_pos == std::string::npos) {
    name_ = path_.substr(0, extension_pos);
    extension_ = path_.substr(extension_pos + 1);
  } else if (extension_pos == std::string::npos) {
    name_ = path_.substr(name_pos + 1);
  } else {
    name_ = path_.substr(name_pos + 1, extension_pos - name_pos - 1);
    extension_ = path_.substr(extension_pos + 1);
  }
}