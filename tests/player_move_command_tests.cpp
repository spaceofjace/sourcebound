//
// Created by Jace Shultz on 6/20/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../../include/gamestate/PlayerMoveCommand.h"
#include "../include/gamestate/GameWorld.h"
#include "mocks/MockCommandQueue.h"
#include "mocks/MockComponentManager.h"
#include "mocks/MockEntityManager.h"
#include "mocks/MockSystemManager.h"
#include "../include/ecs/components/Components.h"

TEST(PlayerMoveCommandTest, AppliesVelocityToPaddleEntity) {
  using namespace sb::gamestate;
  using namespace sb::ecs;

  auto em = std::make_shared<MockEntityManager>();

  //unfortunately need concrete version of ComponentManager for now
  auto cm = std::make_shared<ComponentManager>();

  auto sm = std::make_shared<MockSystemManager>(); // can be an empty stub
  auto cq = std::make_shared<MockCommandQueue>(); // not used in test
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  cm->register_component<Paddle>();
  cm->register_component<Velocity>();

  Entity paddle = gw->create_entity();
  gw->add_component<Paddle>(paddle, Paddle{});
  gw->add_component<Velocity>(paddle, Velocity{0.f, 0.f});

  PlayerMoveCommand move_cmd(1.5f, -2.0f);
  move_cmd.apply(gw);

  const auto& velocity = gw->get_component<Velocity>(paddle);
  EXPECT_FLOAT_EQ(velocity.x, 1.5f);
  EXPECT_FLOAT_EQ(velocity.y, -2.0f);
}

TEST(PlayerMoveCommandTest, AppliesVelocityToPaddleAndStuckBallEntities) {
  using namespace sb::gamestate;
  using namespace sb::ecs;

  auto em = std::make_shared<MockEntityManager>();
  auto cm = std::make_shared<ComponentManager>();
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  cm->register_component<Paddle>();
  cm->register_component<Velocity>();
  cm->register_component<Ball>();
  cm->register_component<StuckToPaddle>();

  // Paddle
  Entity paddle = gw->create_entity();
  gw->add_component<Paddle>(paddle, Paddle{});
  gw->add_component<Velocity>(paddle, Velocity{0.f, 0.f});

  // Ball (stuck to paddle)
  Entity ball = gw->create_entity();
  gw->add_component<Ball>(ball, Ball{});
  gw->add_component<StuckToPaddle>(ball, StuckToPaddle{});
  gw->add_component<Velocity>(ball, Velocity{0.f, 0.f});

  PlayerMoveCommand move_cmd(1.0f, 0.0f);
  move_cmd.apply(gw);

  const auto& paddle_vel = gw->get_component<Velocity>(paddle);
  const auto& ball_vel = gw->get_component<Velocity>(ball);

  EXPECT_FLOAT_EQ(paddle_vel.x, 1.0f);
  EXPECT_FLOAT_EQ(ball_vel.x, 1.0f);
  EXPECT_FLOAT_EQ(paddle_vel.y, 0.0f);
  EXPECT_FLOAT_EQ(ball_vel.y, 0.0f);
}