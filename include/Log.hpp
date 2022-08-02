// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef LOG_HPP_
#define LOG_HPP_

#include <fstream>
#include <string>

namespace ft {
enum LogFlag { LOG_TITLE, LOG_FILE };

const std::string getTimestamp(LogFlag flag = LOG_FILE);

class Log {
 public:
  Log();
  ~Log();

  std::ofstream& getLogStream();
  void           writeTimeLog(const std::string& log);
  void           writeLog(const std::string& log);
  void           writeEndl();

 private:
  std::ofstream log_file_;
};

static Log log;
}  // namespace ft

#endif  // LOG_HPP_
