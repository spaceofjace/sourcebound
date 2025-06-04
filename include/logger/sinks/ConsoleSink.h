//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file ConsoleSink.h
 * @ingroup Logging
 * @brief Concrete implementation of ISink that logs to the console.
 *
 * Sends log messages to either `std::clog` or `std::cerr` based on severity.
 */

#ifndef CONSOLESINK_H
#define CONSOLESINK_H

#include <iostream>

#include "../Logger.h"
#include "ISink.h"

namespace sb::log {

/**
 * @class ConsoleSink
 * @ingroup Logging
 * @brief Outputs log messages to the standard output streams.
 *
 * `Info` and `Debug` messages are sent to `std::clog`, while `Warning` and `Error`
 * messages go to `std::cerr`. Formats messages with a log level prefix.
 */
class ConsoleSink final : public ISink {
public:
  ConsoleSink() = default;

  void log(const Level level, const std::string_view message) override {
    std::ostream& out = (level == Level::Error || level == Level::Warning)
                          ? std::cerr : std::clog;

    switch (level) {
      case Level::Debug:   out << "[DEBUG] "; break;
      case Level::Info:    out << "[INFO] "; break;
      case Level::Warning: out << "[WARN] "; break;
      case Level::Error:   out << "[ERROR] "; break;
    }

    out << message << '\n';
  }
};

}  // namespace sb::log

#endif //CONSOLESINK_H
