//
// Created by Jace Shultz on 7/31/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../include/gamestate/LaunchBallCommand.h"

#include <gtest/gtest.h>
#include "../include/gamestate/GameWorld.h"
#include "../include/math/Vec2.h"
#include "mocks/MockCommandQueue.h"
#include "mocks/MockEntityManager.h"
#include "mocks/MockSystemManager.h"
#include "../include/ecs/components/Components.h"

TEST(LaunchBallCommandTest, LaunchesAllBalls) {
  using namespace sb::gamestate;
  using namespace sb::ecs;

  auto em = std::make_shared<MockEntityManager>();
  auto cm = std::make_shared<ComponentManager>();
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  cm->register_component<Ball>();
  cm->register_component<Direction>();
  cm->register_component<PositionFollower>();

  Entity ball = gw->create_entity();
  gw->add_component<Ball>(ball, Ball{});
  gw->add_component<Direction>(ball, Direction{0.f, 0.f});
  gw->add_component<PositionFollower>(ball, PositionFollower{Entity{123, 0}, sb::math::Vec2{1.F, 2.F}});

  LaunchBallCommand cmd;
  cmd.apply(gw);

  const auto& direction = gw->get_component<Direction>(ball);
  EXPECT_FLOAT_EQ(direction.x, 0.f);
  EXPECT_FLOAT_EQ(direction.y, -1.f);
  EXPECT_FALSE(gw->has_component<PositionFollower>(ball));
}

TEST(LaunchBallCommandTest, IgnoresEntitiesWithoutRequiredComponents) {
  using namespace sb::gamestate;
  using namespace sb::ecs;

  auto em = std::make_shared<MockEntityManager>();
  auto cm = std::make_shared<ComponentManager>();
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  cm->register_component<Ball>();
  cm->register_component<Direction>();
  cm->register_component<PositionFollower>();

  Entity incomplete = gw->create_entity();
  gw->add_component<Ball>(incomplete, Ball{});  // Missing Direction & PositionFollower

  LaunchBallCommand cmd;
  EXPECT_NO_THROW(cmd.apply(gw));
  EXPECT_TRUE(gw->has_component<Ball>(incomplete));
}