//
// Created by Jace Shultz on 7/31/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../include/ecs/systems/FollowSystem.h"
#include "../include/ecs/components/Components.h"
#include "mocks/MockGameDataManager.h"
#include "mocks/MockSink.h"
#include <gtest/gtest.h>

using namespace sb::ecs;

TEST(FollowSystemTest, FollowerTracksTargetWithOffset) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Transform>();
  cm->register_component<PositionFollower>();

  auto mock_data = std::make_shared<MockGameDataManager>();
  auto follow_system = std::make_shared<FollowSystem>(mock_data);

  Entity target{1, 0};
  Entity follower{2, 0};

  cm->add_component<Transform>(target, Transform{{50.0f, 75.0f}, {10.0f, 10.0f}});
  cm->add_component<Transform>(follower, Transform{{0.0f, 0.0f}, {10.0f, 10.0f}});
  cm->add_component<PositionFollower>(follower, PositionFollower{target, sb::math::Vec2{5.0f, -10.0f}});

  follow_system->entities.insert(follower);
  follow_system->update(0.0f, *cm);

  const auto& updated = cm->get_component<Transform>(follower).position;
  EXPECT_FLOAT_EQ(updated.x, 55.0f);  // 50 + 5
  EXPECT_FLOAT_EQ(updated.y, 65.0f);  // 75 - 10
}

TEST(FollowSystemTest, SkipsFollowerIfTargetHasNoTransform) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Transform>();
  cm->register_component<PositionFollower>();
  cm->register_component<CircleCollider>();

  auto mock_data = std::make_shared<MockGameDataManager>();
  auto follow_system = std::make_shared<FollowSystem>(mock_data);

  auto mock_sink = std::make_shared<MockSink>();
  Logger::set_sink(mock_sink);

  Entity target{1, 0};
  Entity follower{2, 0};

  cm->add_component<Transform>(follower, Transform{{0.0f, 0.0f}, {10.0f, 10.0f}});
  cm->add_component<PositionFollower>(follower, PositionFollower{target, sb::math::Vec2{2.0f, 2.0f}});

  follow_system->entities.insert(follower);
  follow_system->update(0.0f, *cm);

  const auto& updated = cm->get_component<Transform>(follower).position;
  EXPECT_FLOAT_EQ(updated.x, 0.0f);
  EXPECT_FLOAT_EQ(updated.y, 0.0f);
  EXPECT_EQ(mock_sink->last_level, sb::log::Level::Warning);
}