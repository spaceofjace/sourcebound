//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @defgroup Logging Logging System
 * @brief Provides modular and configurable logging tools.
 *
 * This module includes a flexible logging interface and sink system.  Messages can be logged at
 * various severity levels and routed to custom output sinks such as console, files, or external
 * monitors.
 */

/**
 * @file Logger.h
 * @ingroup Logging
 * @brief Central static logging interface for the application.
 *
 * Provides level-based logging (debug, info, warning, error) and routes messages
 * to the currently assigned sink. This class is non-instantiable and internally
 * manages a single static sink instance.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <string_view>
#include "sinks/ISink.h"

namespace sb::log {

/**
 * @class Logger
 * @ingroup Logging
 * @brief Static interface for application-wide logging.
 *
 * Logger allows logging messages at various levels of severity. Internally, it
 * delegates to an `ISink` implementation to handle message formatting and output.
 */
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
