#include "FileManager.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Utilities.hpp"

FileManager::FileManager() {}
FileManager::FileManager(const PathType& root, const PathType& path)
    : path_(root + path) {
  parsePath(path);
}
FileManager::FileManager(const PathType& path) : path_(path) {}
FileManager::~FileManager() {}

const FileManager::PathType&      FileManager::getPath() const { return path_; }
const FileManager::NameType&      FileManager::getName() const { return name_; }
const FileManager::ExtensionType& FileManager::getExtension() const {
  return extension_;
}

void FileManager::setPath(const PathType& path) { path_ = path; }
void FileManager::setName(const NameType& name) { name_ = name; }
void FileManager::setExtension(const ExtensionType& extension) {
  extension_ = extension;
}

bool FileManager::remove() { return ::unlink(path_.c_str()); }

bool FileManager::isExist() const {
  struct stat buf;
  return ::stat(path_.c_str(), &buf) == 0;
}

bool FileManager::isDirectory() const {
  struct stat buf;
  return ::stat(path_.c_str(), &buf) == 0 && S_ISDIR(buf.st_mode);
}

bool FileManager::isExist(const PathType& path) {
  struct stat buf;
  return ::stat(path.c_str(), &buf) == 0;
}

bool FileManager::isDirectory(const PathType& path) {
  struct stat buf;
  return ::stat(path.c_str(), &buf) == 0 && S_ISDIR(buf.st_mode);
}

const std::string FileManager::getContent() const {
  std::ifstream     ifs;
  std::stringstream ss;

  ifs.open(path_.c_str(), std::ifstream::in);
  ss << ifs.rdbuf();
  ifs.close();

  return ss.str();
}

void FileManager::appendContent(const std::string& content) {
  std::ofstream ofs;

  ofs.open(path_.c_str(), std::ofstream::out | std::ofstream::app);
  ofs << content;
  ofs.close();
}

void FileManager::createFile(const std::string& content) {
  std::ofstream ofs;

  ofs.open(path_.c_str(), std::ofstream::out);
  ofs << content;
  ofs.close();
}

void FileManager::parsePath(const PathType& path) {
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