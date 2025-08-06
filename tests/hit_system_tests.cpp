//
// Created by Jace Shultz on 8/5/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//
#include <gtest/gtest.h>

#include "../include/ecs/components/Components.h"
#include "../include/ecs/systems/HitSystem.h"
#include "mocks/MockCommandQueue.h"
#include "mocks/MockComponentManager.h"
#include "mocks/MockEntityManager.h"
#include "mocks/MockGameDataManager.h"
#include "mocks/MockSystemManager.h"

using namespace sb::ecs;
using namespace sb::gamestate;

TEST(HitSystemTest, DecrementsHitsIfNotFatal) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Velocity>();
  cm->register_component<Direction>();
  cm->register_component<Transform>();
  cm->register_component<Paddle>();
  cm->register_component<Ball>();
  cm->register_component<HitsRequired>();
  cm->register_component<PendingHit>();
  cm->register_component<PendingDestroy>();

  auto mock_data = std::make_shared<MockGameDataManager>();

  //unfortunately need concrete version of ComponentManager and GameWorld for now
  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>(); // can be an empty stub
  auto cq = std::make_shared<MockCommandQueue>(); // not used in test
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  auto hit_system = std::make_shared<HitSystem>(mock_data);

  Entity e = em->create_entity();
  cm->add_component(e, HitsRequired{3});
  cm->add_component(e, PendingHit{1});
  hit_system->entities.insert(e);

  hit_system->update(0.0f, *gw);

  EXPECT_EQ(cm->get_component<HitsRequired>(e).hits, 2);
  EXPECT_FALSE(cm->has_component<PendingDestroy>(e));
  EXPECT_FALSE(cm->has_component<PendingHit>(e));
}

TEST(HitSystemTest, AddsPendingDestroyIfHitsDepleted) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Velocity>();
  cm->register_component<Direction>();
  cm->register_component<Transform>();
  cm->register_component<Paddle>();
  cm->register_component<Ball>();
  cm->register_component<HitsRequired>();
  cm->register_component<PendingHit>();
  cm->register_component<PendingDestroy>();

  auto mock_data = std::make_shared<MockGameDataManager>();

  //unfortunately need concrete version of ComponentManager and GameWorld for now
  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>(); // can be an empty stub
  auto cq = std::make_shared<MockCommandQueue>(); // not used in test
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  auto hit_system = std::make_shared<HitSystem>(mock_data);

  Entity e = em->create_entity();
  cm->add_component(e, HitsRequired{2});
  cm->add_component(e, PendingHit{3});
  hit_system->entities.insert(e);

  hit_system->update(0.0f, *gw);

  EXPECT_TRUE(cm->has_component<PendingDestroy>(e));
  EXPECT_FALSE(cm->has_component<PendingHit>(e));
}