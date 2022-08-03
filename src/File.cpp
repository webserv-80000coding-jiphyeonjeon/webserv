#include "File.hpp"

File::File(const PathType& path) : path_(path), fd_(-1) {}
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