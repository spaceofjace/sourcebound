//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file ISink.h
 * @ingroup Logging
 * @brief Declares the logging sink interface.
 *
 * Sinks define how log messages are processed and where they are routed (e.g., to console, file, etc.).
 */

#ifndef ISINK_H
#define ISINK_H
#include <string_view>

namespace sb::log {

enum class Level {
  Debug,
  Info,
  Warning,
  Error
};

/**
 * @interface ISink
 * @ingroup Logging
 * @brief Interface for all logging sinks.
 *
 * A sink defines how log messages are output. Typical implementations include console, file, or
 * external systems. This interface is non-copyable and must be subclassed.
 */
class ISink {
public:
  ISink() = default;
  virtual ~ISink() = default;

  ISink(const ISink&) = delete;
  ISink& operator=(const ISink&) = delete;
  ISink(ISink&&) = delete;
  ISink& operator=(ISink&&) = delete;

  virtual void log(Level level, std::string_view message) = 0;
};

}  // namespace sb::log
#endif //ISINK_H
