//
// Created by Jace Shultz on 8/5/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//
#include <gtest/gtest.h>

#include "../include/ecs/components/Components.h"
#include "../include/ecs/systems/TriggerSystem.h"
#include "mocks/MockCommandQueue.h"
#include "mocks/MockComponentManager.h"
#include "mocks/MockEntityManager.h"
#include "mocks/MockGameDataManager.h"
#include "mocks/MockSystemManager.h"

using namespace sb::ecs;
using namespace sb::gamestate;

TEST(TriggerSystemTest, AddsPendingHitWhenTriggeredWithHitType) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Velocity>();
  cm->register_component<Direction>();
  cm->register_component<Transform>();
  cm->register_component<Paddle>();
  cm->register_component<Ball>();
  cm->register_component<WasTriggered>();
  cm->register_component<PendingHit>();
  cm->register_component<Triggerable>();

  auto mock_data = std::make_shared<MockGameDataManager>();

  //unfortunately need concrete version of ComponentManager and GameWorld for now
  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>(); // can be an empty stub
  auto cq = std::make_shared<MockCommandQueue>(); // not used in test
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  Entity e = em->create_entity();
  cm->add_component(e, Triggerable{{TriggerType::Hit}});
  cm->add_component(e, WasTriggered{Entity{42}});

  auto trigger_system = std::make_shared<TriggerSystem>(mock_data);

  trigger_system->entities.insert(e);

  trigger_system->update(0.0f, *gw);

  EXPECT_TRUE(cm->has_component<PendingHit>(e));
  EXPECT_FALSE(cm->has_component<WasTriggered>(e));
}

TEST(TriggerSystemTest, AddsPendingDestroyToSourceWhenTriggeredWithDestroyType) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Velocity>();
  cm->register_component<Direction>();
  cm->register_component<Transform>();
  cm->register_component<Paddle>();
  cm->register_component<Ball>();
  cm->register_component<WasTriggered>();
  cm->register_component<PendingHit>();
  cm->register_component<Triggerable>();
  cm->register_component<PendingDestroy>();

  auto mock_data = std::make_shared<MockGameDataManager>();

  //unfortunately need concrete version of ComponentManager and GameWorld for now
  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>(); // can be an empty stub
  auto cq = std::make_shared<MockCommandQueue>(); // not used in test
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  auto trigger_system = std::make_shared<TriggerSystem>(mock_data);

  Entity source = em->create_entity();
  Entity e = em->create_entity();

  cm->add_component(e, Triggerable{{TriggerType::Destroy}});
  cm->add_component(e, WasTriggered{source});
  trigger_system->entities.insert(e);

  trigger_system->update(0.0f, *gw);

  EXPECT_TRUE(cm->has_component<PendingDestroy>(source));
  EXPECT_FALSE(cm->has_component<WasTriggered>(e));
}