//
// Created by Jace Shultz on 8/5/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>

#include "../include/ecs/components/Components.h"
#include "../include/ecs/systems/DestructionSystem.h"
#include "mocks/MockCommandQueue.h"
#include "mocks/MockComponentManager.h"
#include "mocks/MockEntityManager.h"
#include "mocks/MockGameDataManager.h"
#include "mocks/MockSystemManager.h"

using namespace sb::ecs;
using namespace sb::gamestate;

TEST(DestructionSystemTest, DestroysEntityWithoutIndestructible) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Velocity>();
  cm->register_component<Direction>();
  cm->register_component<Transform>();
  cm->register_component<Paddle>();
  cm->register_component<Ball>();
  cm->register_component<Indestructible>();
  cm->register_component<PendingHit>();
  cm->register_component<PendingDestroy>();
  cm->register_component<PlayerSessionState>();
  cm->register_component<ScoreValue>();

  auto mock_data = std::make_shared<MockGameDataManager>();

  //unfortunately need concrete version of ComponentManager and GameWorld for now
  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>(); // can be an empty stub
  auto cq = std::make_shared<MockCommandQueue>(); // not used in test
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  auto destruction_system = std::make_shared<DestructionSystem>(mock_data);

  Entity e = gw->create_entity();
  cm->add_component(e, PendingDestroy{});
  destruction_system->entities.insert(e);

  destruction_system->update(0.0f, *gw);

  EXPECT_FALSE(gw->is_alive(e));
}

TEST(DestructionSystemTest, SkipsDestroyingIfIndestructible) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Velocity>();
  cm->register_component<Direction>();
  cm->register_component<Transform>();
  cm->register_component<Paddle>();
  cm->register_component<Ball>();
  cm->register_component<Indestructible>();
  cm->register_component<PendingHit>();
  cm->register_component<PendingDestroy>();
  cm->register_component<PlayerSessionState>();
  cm->register_component<ScoreValue>();

  auto mock_data = std::make_shared<MockGameDataManager>();

  //unfortunately need concrete version of ComponentManager and GameWorld for now
  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>(); // can be an empty stub
  auto cq = std::make_shared<MockCommandQueue>(); // not used in test
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  auto destruction_system = std::make_shared<DestructionSystem>(mock_data);

  Entity e = gw->create_entity();

  gw->add_component(e, PendingDestroy{});
  gw->add_component(e, Indestructible{});
  destruction_system->entities.insert(e);

  destruction_system->update(0.0f, *gw);

  EXPECT_TRUE(gw->is_alive(e));
  EXPECT_FALSE(cm->has_component<PendingDestroy>(e));
}

TEST(DestructionSystemTest, AwardsScoreToPlayerSessionOnDestroy) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Velocity>();
  cm->register_component<Direction>();
  cm->register_component<Transform>();
  cm->register_component<Paddle>();
  cm->register_component<Ball>();
  cm->register_component<Indestructible>();
  cm->register_component<PendingHit>();
  cm->register_component<PendingDestroy>();
  cm->register_component<PlayerSessionState>();
  cm->register_component<ScoreValue>();

  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();
  auto data = std::make_shared<MockGameDataManager>();
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  Entity session = gw->create_entity();
  gw->add_component(session, PlayerSessionState{});

  Entity brick = gw->create_entity();
  gw->add_component(brick, PendingDestroy{});
  gw->add_component(brick, ScoreValue{250});

  auto destruction_system = std::make_shared<DestructionSystem>(data);
  destruction_system->entities.insert(brick);

  destruction_system->update(0.0f, *gw);

  // Verify score was added
  const auto& session_data = gw->get_component<PlayerSessionState>(session);
  EXPECT_EQ(session_data.score, 250);
  EXPECT_FALSE(gw->is_alive(brick));
}