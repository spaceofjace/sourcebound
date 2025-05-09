//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

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
