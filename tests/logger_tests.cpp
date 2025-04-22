//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>
#include <sstream>
#include <memory>
#include "..\include\logger\Logger.h"
#include "..\include\logger\sinks\ISink.h"

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

TEST(LoggerTest, LogsDebugMessage) {
  auto mock = std::make_shared<MockSink>();
  Logger::set_sink(mock);

  Logger::debug("Debugging");

  EXPECT_EQ(mock->last_level, Level::Debug);
  EXPECT_EQ(mock->last_message, "Debugging");
}

TEST(LoggerTest, LogsInfoMessage) {
  auto mock = std::make_shared<MockSink>();
  Logger::set_sink(mock);

  Logger::info("Information");

  EXPECT_EQ(mock->last_level, Level::Info);
  EXPECT_EQ(mock->last_message, "Information");
}

TEST(LoggerTest, LogsWarningMessage) {
  auto mock = std::make_shared<MockSink>();
  Logger::set_sink(mock);

  Logger::warn("Warning!");

  EXPECT_EQ(mock->last_level, Level::Warning);
  EXPECT_EQ(mock->last_message, "Warning!");
}

TEST(LoggerTest, LogsErrorMessage) {
  auto mock = std::make_shared<MockSink>();
  Logger::set_sink(mock);

  Logger::error("Something broke");

  EXPECT_EQ(mock->last_level, Level::Error);
  EXPECT_EQ(mock->last_message, "Something broke");
}

TEST(LoggerTest, ReplacesSinkCorrectly) {
  auto mock1 = std::make_shared<MockSink>();
  auto mock2 = std::make_shared<MockSink>();

  Logger::set_sink(mock1);
  Logger::info("One");
  EXPECT_EQ(mock1->last_message, "One");

  Logger::set_sink(mock2);
  Logger::info("Two");
  EXPECT_EQ(mock2->last_message, "Two");

  // Make sure the first sink is not reused
  EXPECT_EQ(mock1->last_message, "One");
}
