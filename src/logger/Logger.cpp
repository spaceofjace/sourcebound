//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/logger/Logger.h"
#include "../../include/logger/sinks/ConsoleSink.h"

namespace sb::log {

Logger::Logger() : sink_(std::make_shared<ConsoleSink>()) {}

Logger& Logger::get_instance() {
  static Logger instance;
  return instance;
}

void Logger::set_sink(std::shared_ptr<ISink> sink) {
  get_instance().sink_ = std::move(sink);
}

void Logger::log(const Level level, const std::string_view message) {
  get_instance().sink_->log(level, message);
}

void Logger::debug(const std::string_view msg) { log(Level::Debug, msg); }
void Logger::info(const std::string_view msg)  { log(Level::Info, msg); }
void Logger::warn(const std::string_view msg)  { log(Level::Warning, msg); }
void Logger::error(const std::string_view msg) { log(Level::Error, msg); }

}  // namespace sb::log