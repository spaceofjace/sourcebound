//
// Created by Jace Shultz on 7/25/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../include/ecs/systems/PhysicsSystem.h"
#include "../include/ecs/components/Components.h"
#include "mocks/MockComponentManager.h"
#include "mocks/MockGameDataManager.h"
#include <gtest/gtest.h>

using namespace sb::ecs;

TEST(PhysicsSystemTest, MovesPaddleAndStuckBallCorrectly) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Velocity>();
  cm->register_component<Transform>();
  cm->register_component<Paddle>();
  cm->register_component<Ball>();
  cm->register_component<StuckToPaddle>();

  auto mock_data = std::make_shared<MockGameDataManager>();

  auto physics = std::make_shared<PhysicsSystem>(mock_data, cm);

  Entity paddle{1};
  cm->add_component<Velocity>(paddle, Velocity{1.0f, 0.0f});
  cm->add_component<Transform>(paddle, Transform{{0.0f, 0.0f}, {20.0f, 10.0f}});
  cm->add_component<Paddle>(paddle, Paddle{});
  physics->entities.insert(paddle);

  Entity stuck_ball{2};
  cm->add_component<Velocity>(stuck_ball, Velocity{1.0f, 0.0f});
  cm->add_component<Transform>(stuck_ball, Transform{{100.0f, 0.0f}, {10.0f, 10.0f}});
  cm->add_component<Ball>(stuck_ball, Ball{});
  cm->add_component<StuckToPaddle>(stuck_ball, StuckToPaddle{});
  physics->entities.insert(stuck_ball);

  Entity launched_ball{3};
  cm->add_component<Velocity>(launched_ball, Velocity{1.0f, 0.0f});
  cm->add_component<Transform>(launched_ball, Transform{{200.0f, 0.0f}, {10.0f, 10.0f}});
  cm->add_component<Ball>(launched_ball, Ball{});
  physics->entities.insert(launched_ball);

  physics->update(1.0f);  // delta_time = 1.0s

  const auto& paddle_x = cm->get_component<Transform>(paddle).position.x;
  const auto& stuck_x = cm->get_component<Transform>(stuck_ball).position.x;
  const auto& launched_x = cm->get_component<Transform>(launched_ball).position.x;

  EXPECT_FLOAT_EQ(paddle_x, 200.0f);   // 200 * 1.0
  EXPECT_FLOAT_EQ(stuck_x, 300.0f);    // 200 * 1.0 (still uses paddle speed)
  EXPECT_FLOAT_EQ(launched_x, 400.0f); // 400 * 1.0
}