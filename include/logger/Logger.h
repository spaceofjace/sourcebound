//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <string_view>
#include "sinks/ISink.h"

namespace sb::log {

class Logger {
public:
  static void set_sink(std::shared_ptr<ISink> sink);

  static void log(Level level, std::string_view message);

  static void debug(std::string_view msg);
  static void info(std::string_view msg);
  static void warn(std::string_view msg);
  static void error(std::string_view msg);

private:
  Logger();

  static Logger& get_instance();

  std::shared_ptr<ISink> sink_;
};

}  // namespace sb::log

#endif //LOGGER_H
