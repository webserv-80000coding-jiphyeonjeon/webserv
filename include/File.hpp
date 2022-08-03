#ifndef WEBSERV_FILE_HPP
#define WEBSERV_FILE_HPP

#include <sys/stat.h>

#include <string>

class File {
 public:
  typedef std::string PathType;
  typedef std::string NameType;
  typedef std::string ExtensionType;
  typedef int         FdType;

  File(const PathType& path);
  ~File();

  const PathType&      getPath() const;
  const NameType&      getName() const;
  const ExtensionType& getExtension() const;
  const FdType&        getFd() const;

  void setPath(const PathType& path);
  void setName(const NameType& name);
  void setExtension(const ExtensionType& extension);
  void setFd(const FdType& fd);

  bool open();
  bool close();
  bool create();
  bool remove();

  bool isExist() const;
  bool isDirectory() const;

  static bool isExist(const PathType& path);
  static bool isDirectory(const PathType& path);

 private:
  void parsePath();

  PathType      path_;
  NameType      name_;
  ExtensionType extension_;
  FdType        fd_;
};

#endif