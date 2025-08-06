//
// Created by Jace Shultz on 7/25/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>
#include "../../../../include/data/HardcodedGameDataManager.h"

using namespace sb::data;

TEST(HardcodedGameDataManagerTest, ReturnsTrueOnLoad) {
  HardcodedGameDataManager manager;
  EXPECT_TRUE(manager.load_config("ignored"));
}

TEST(HardcodedGameDataManagerTest, LoadsCorrectLevelAfterSet) {
  HardcodedGameDataManager manager;
  manager.load_config("ignored");
  manager.set_current_level(1);
  const auto& level = manager.get_current_level_data();
  EXPECT_EQ(level.level_id, 1);
  EXPECT_EQ(level.level_name, "Level1");
}

TEST(HardcodedGameDataManagerTest, FallsBackOnInvalidLevel) {
  HardcodedGameDataManager manager;
  manager.load_config("ignored");
  manager.set_current_level(999);  // nonexistent
  const auto& level = manager.get_current_level_data();
  EXPECT_EQ(level.level_id, HardcodedGameDataManager::kFallbackLevelId);
}