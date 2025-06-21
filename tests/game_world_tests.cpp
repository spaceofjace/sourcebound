//
// Created by Jace Shultz on 5/8/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>

#include "../include/ecs/ISystem.h"
#include "../include/gamestate/GameWorld.h"
#include "mocks/MockCommandQueue.h"
#include "mocks/MockComponentManager.h"
#include "mocks/MockEntityManager.h"
#include "mocks/MockSystemManager.h"

using namespace sb::gamestate;

TEST(GameWorldTest, CanCreateAndDestroyEntity) {
  auto em = std::make_shared<MockEntityManager>();
  auto cm = std::make_shared<ComponentManager>(); //templated so requires the concrete impl
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();

  GameWorld gw(em, cm, sm, cq);

  Entity e = gw.create_entity();
  EXPECT_TRUE(em->is_alive(e));

  gw.destroy_entity(e);
  EXPECT_FALSE(em->is_alive(e));
}

TEST(GameWorldTest, CanAddAndCheckComponent) {
  auto em = std::make_shared<MockEntityManager>();
  auto cm = std::make_shared<ComponentManager>(); //templated so requires the concrete impl
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();

  GameWorld gw(em, cm, sm, cq);
  Entity e = gw.create_entity();

  DummyComponent comp{123};
  gw.register_component<DummyComponent>();
  gw.add_component<DummyComponent>(e, comp);

  EXPECT_TRUE(gw.has_component<DummyComponent>(e));
  EXPECT_EQ(gw.get_component<DummyComponent>(e).value, 123);
}

TEST(GameWorldTest, CanRemoveComponent) {
  auto em = std::make_shared<MockEntityManager>();
  auto cm = std::make_shared<ComponentManager>(); //templated so requires the concrete impl
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();

  GameWorld gw(em, cm, sm, cq);
  Entity e = gw.create_entity();

  DummyComponent comp{999};
  gw.register_component<DummyComponent>();
  gw.add_component<DummyComponent>(e, comp);
  EXPECT_TRUE(gw.has_component<DummyComponent>(e));

  gw.remove_component<DummyComponent>(e);
  EXPECT_FALSE(gw.has_component<DummyComponent>(e));
}

TEST(GameWorldTest, UpdateDelegatesToSystemManager) {
  auto em = std::make_shared<MockEntityManager>();
  auto cm = std::make_shared<ComponentManager>();

  struct TestSystemManager : MockSystemManager {
    bool updated = false;
    void update(float) override { updated = true; }
  };
  auto sm = std::make_shared<TestSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();

  GameWorld gw(em, cm, sm, cq);
  gw.update(0.016f);

  EXPECT_TRUE(sm->updated);
}