#pragma once

#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <sstream>
#include <fstream>

class LogDuration {
public:
  LogDuration(const std::string& filename = "logs/out.txt",
              const std::string& msg = "")
    : message(msg)
    , filename(filename)
    , start(std::chrono::steady_clock::now())
  {
  }

  ~LogDuration() {
    auto finish = std::chrono::steady_clock::now();
    auto dur = finish - start;

    std::ofstream output_file;
    output_file.open(filename);
    output_file << message
       << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()
       << " ms" << std::endl;
    output_file.close();
  }
private:
  std::string message;
  std::string filename;
  std::chrono::steady_clock::time_point start;
};

#ifndef UNIQ_ID
  #define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
  #define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)
#endif

#define LOG_DURATION(message) \
  LogDuration UNIQ_ID(__LINE__){message};
