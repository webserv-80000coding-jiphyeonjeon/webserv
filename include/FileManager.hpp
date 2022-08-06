#ifndef WEBSERV_FILEMANAGER_HPP
#define WEBSERV_FILEMANAGER_HPP

#include <sys/stat.h>

#include <fstream>
#include <string>

class FileManager {
 public:
  typedef std::string PathType;
  typedef std::string NameType;
  typedef std::string ExtensionType;

  FileManager();
  FileManager(const PathType& root, const PathType& path);
  FileManager(const PathType& path);
  ~FileManager();

  const PathType&      getPath() const;
  const NameType&      getName() const;
  const ExtensionType& getExtension() const;
  const NameType       getFullName() const;

  void setPath(const PathType& path);
  void setName(const NameType& name);
  void setExtension(const ExtensionType& extension);

  bool remove();

  bool isExist() const;
  bool isDirectory() const;

  static bool isExist(const PathType& path);
  static bool isDirectory(const PathType& path);

  const std::string getContent() const;
  void              appendContent(const std::string& content);
  void              createFile(const std::string& content);

 private:
  void parsePath();

  PathType      path_;
  NameType      name_;
  ExtensionType extension_;
};

#endif
