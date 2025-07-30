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

TEST(PlayerMoveCommandTest, AppliesDirectionToPaddleEntity) {
  using namespace sb::gamestate;
  using namespace sb::ecs;

  auto em = std::make_shared<MockEntityManager>();

  //unfortunately need concrete version of ComponentManager for now
  auto cm = std::make_shared<ComponentManager>();

  auto sm = std::make_shared<MockSystemManager>(); // can be an empty stub
  auto cq = std::make_shared<MockCommandQueue>(); // not used in test
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  cm->register_component<Paddle>();
  cm->register_component<Direction>();
  cm->register_component<StuckToPaddle>();

  Entity paddle = gw->create_entity();
  gw->add_component<Paddle>(paddle, Paddle{});
  gw->add_component<Direction>(paddle, Direction{0.f, 0.f});

  PlayerMoveCommand move_cmd(1.0f, 0);
  move_cmd.apply(gw);

  const auto& direction = gw->get_component<Direction>(paddle);
  EXPECT_FLOAT_EQ(direction.x, 1.0f);
  EXPECT_FLOAT_EQ(direction.y, 0);
}

TEST(PlayerMoveCommandTest, AppliesDirectionToPaddleAndStuckBallEntities) {
  using namespace sb::gamestate;
  using namespace sb::ecs;

  auto em = std::make_shared<MockEntityManager>();
  auto cm = std::make_shared<ComponentManager>();
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  cm->register_component<Paddle>();
  cm->register_component<Direction>();
  cm->register_component<Ball>();
  cm->register_component<StuckToPaddle>();

  // Paddle
  Entity paddle = gw->create_entity();
  gw->add_component<Paddle>(paddle, Paddle{});
  gw->add_component<Direction>(paddle, Direction{0.f, 0.f});

  // Ball (stuck to paddle)
  Entity ball = gw->create_entity();
  gw->add_component<Ball>(ball, Ball{});
  gw->add_component<StuckToPaddle>(ball, StuckToPaddle{});
  gw->add_component<Direction>(ball, Direction{0.f, 0.f});

  PlayerMoveCommand move_cmd(1.0f, 0.0f);
  move_cmd.apply(gw);

  const auto& paddle_vel = gw->get_component<Direction>(paddle);
  const auto& ball_vel = gw->get_component<Direction>(ball);

  EXPECT_FLOAT_EQ(paddle_vel.x, 1.0f);
  EXPECT_FLOAT_EQ(ball_vel.x, 1.0f);
  EXPECT_FLOAT_EQ(paddle_vel.y, 0.0f);
  EXPECT_FLOAT_EQ(ball_vel.y, 0.0f);
}