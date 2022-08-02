#include "Log.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

const std::string ft::getTimestamp(ft::LogFlag flag) {
  time_t rawtime;
  time(&rawtime);

  struct tm* timeinfo;
  timeinfo = localtime(&rawtime);

  std::stringstream timestamp;
  if (flag == ft::LOG_TITLE) {
    timestamp << std::setfill('0') << std::setw(4) << timeinfo->tm_year + 1900
              << std::setw(2) << timeinfo->tm_mon + 1 << std::setw(2)
              << timeinfo->tm_mday << "_" << std::setw(2) << timeinfo->tm_hour
              << std::setw(2) << timeinfo->tm_min << std::setw(2)
              << timeinfo->tm_sec;
  } else {
    timestamp << "[" << std::setfill('0') << std::setw(4)
              << timeinfo->tm_year + 1900 << "/" << std::setw(2)
              << timeinfo->tm_mon + 1 << "/" << std::setw(2)
              << timeinfo->tm_mday << " " << std::setw(2) << timeinfo->tm_hour
              << ":" << std::setw(2) << timeinfo->tm_min << ":" << std::setw(2)
              << timeinfo->tm_sec << "] ";
  }

  return timestamp.str();
}

ft::Log::Log() {
  log_file_.open(getTimestamp(ft::LOG_TITLE) + ".log",
                 std::ofstream::out | std::ofstream::app);
}

ft::Log::~Log() {}

std::ofstream& ft::Log::getLogStream() { return log_file_; }

void ft::Log::writeTimeLog(const std::string& log) {
  log_file_ << getTimestamp() << log << std::endl;
}

void ft::Log::writeLog(const std::string& log) {
  log_file_ << log << std::endl;
}

void ft::Log::writeEndl() { log_file_ << std::endl; }
