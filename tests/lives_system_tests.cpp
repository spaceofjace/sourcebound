//
// Created by Jace Shultz on 8/6/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>

#include "../include/ecs/components/Components.h"
#include "../include/ecs/systems/LivesSystem.h"
#include "mocks/MockCommandQueue.h"
#include "mocks/MockEntityManager.h"
#include "mocks/MockGameDataManager.h"
#include "mocks/MockSystemManager.h"

using namespace sb::ecs;
using namespace sb::gamestate;

TEST(LivesSystemTest, DecrementsLifeAndRequestsResetIfLivesRemain) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Velocity>();
  cm->register_component<Direction>();
  cm->register_component<Transform>();
  cm->register_component<Paddle>();
  cm->register_component<Ball>();
  cm->register_component<Indestructible>();
  cm->register_component<PendingHit>();
  cm->register_component<PlayerSessionState>();

  auto mock_data = std::make_shared<MockGameDataManager>();

  //unfortunately need concrete version of ComponentManager and GameWorld for now
  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>(); // can be an empty stub
  auto cq = std::make_shared<MockCommandQueue>(); // not used in test
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  gw->set_stage_lifecycle_state(StageLifecycleState::Active);

  Entity player = gw->create_entity();
  PlayerSessionState p;
  p.score = 0;
  p.lives = 2;
  gw->add_component(player, p);

  auto system = std::make_shared<LivesSystem>(std::make_shared<MockGameDataManager>());
  system->update(0.0f, *gw);

  auto& state = gw->get_component<PlayerSessionState>(player);
  EXPECT_EQ(state.lives, 1);
  EXPECT_EQ(gw->get_stage_lifecycle_state(), StageLifecycleState::PlayerResetRequested);
}

TEST(LivesSystemTest, TriggersGameOverIfLivesReachZero) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Velocity>();
  cm->register_component<Direction>();
  cm->register_component<Transform>();
  cm->register_component<Paddle>();
  cm->register_component<Ball>();
  cm->register_component<Indestructible>();
  cm->register_component<PendingHit>();
  cm->register_component<PlayerSessionState>();

  auto mock_data = std::make_shared<MockGameDataManager>();

  //unfortunately need concrete version of ComponentManager and GameWorld for now
  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>(); // can be an empty stub
  auto cq = std::make_shared<MockCommandQueue>(); // not used in test
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  gw->set_stage_lifecycle_state(StageLifecycleState::Active);

  Entity player = gw->create_entity();
  PlayerSessionState p;
  p.score = 0;
  p.lives = 1;
  gw->add_component(player, p);

  auto system = std::make_shared<LivesSystem>(std::make_shared<MockGameDataManager>());
  system->update(0.0f, *gw);

  auto& state = gw->get_component<PlayerSessionState>(player);
  EXPECT_EQ(state.lives, 0);
  EXPECT_EQ(gw->get_stage_lifecycle_state(), StageLifecycleState::GameOver);
}

TEST(LivesSystemTest, DoesNothingIfBallStillExists) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Velocity>();
  cm->register_component<Direction>();
  cm->register_component<Transform>();
  cm->register_component<Paddle>();
  cm->register_component<Ball>();
  cm->register_component<Indestructible>();
  cm->register_component<PendingHit>();
  cm->register_component<PlayerSessionState>();

  auto mock_data = std::make_shared<MockGameDataManager>();

  //unfortunately need concrete version of ComponentManager and GameWorld for now
  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>(); // can be an empty stub
  auto cq = std::make_shared<MockCommandQueue>(); // not used in test
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  Entity player = gw->create_entity();
  Entity ball = gw->create_entity();

  PlayerSessionState p;
  p.score = 0;
  p.lives = 3;
  gw->add_component(player, p);
  gw->add_component(ball, Ball{});

  auto system = std::make_shared<LivesSystem>(std::make_shared<MockGameDataManager>());

  gw->set_stage_lifecycle_state(StageLifecycleState::Active);
  system->entities.insert(ball);
  system->update(0.0f, *gw);

  auto& state = gw->get_component<PlayerSessionState>(player);
  EXPECT_EQ(state.lives, 3);
  EXPECT_EQ(gw->get_stage_lifecycle_state(), StageLifecycleState::Active);
}