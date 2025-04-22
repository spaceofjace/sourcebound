//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef MOCKSINK_H
#define MOCKSINK_H

#include <string>
#include "../../include/logger/sinks/ISink.h"

using namespace sb::log;

class MockSink : public ISink {
public:
  MockSink() = default;

  void log(const Level level, const std::string_view message) override {
    last_level = level;
    last_message = std::string(message);
  }

  Level last_level;
  std::string last_message;
};
#endif //MOCKSINK_H
