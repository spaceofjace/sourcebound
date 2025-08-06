//
// Created by Jace Shultz on 5/8/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>

#include "../include/ecs/ISystem.h"
#include "../include/ecs/components/Components.h"
#include "../include/gamestate/GameWorld.h"
#include "mocks/MockCommandQueue.h"
#include "mocks/MockComponentManager.h"
#include "mocks/MockEntityManager.h"
#include "mocks/MockGameDataManager.h"
#include "mocks/MockRenderer.h"
#include "mocks/MockSink.h"
#include "mocks/MockSystem.h"
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
    void update_all(float, GameWorld&) override { updated = true; } // <-- updated signature
  };

  auto sm = std::make_shared<TestSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();

  GameWorld gw(em, cm, sm, cq);
  gw.update(0.016f);

  EXPECT_TRUE(sm->updated);
}

TEST(GameWorldTest, RequestExitSetsFlag) {
  auto em = std::make_shared<MockEntityManager_GMock>();
  auto cm = std::make_shared<ComponentManager>();
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();
  GameWorld gw(em, cm, sm, cq);

  EXPECT_FALSE(gw.get_stage_lifecycle_state() == StageLifecycleState::Quit);
  gw.set_stage_lifecycle_state(StageLifecycleState::Quit);
  EXPECT_TRUE(gw.get_stage_lifecycle_state() == StageLifecycleState::Quit);
}

TEST(GameWorldTest, LoadLevelProducesRenderableEntities) {
  using namespace testing;

  auto em = std::make_shared<MockEntityManager_GMock>();
  auto cm = std::make_shared<ComponentManager>();
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();
  auto render_system = std::make_shared<MockSystem>();
  auto data_manager = std::make_shared<MockGameDataManager>();

  //Lots of erroneous Component warnings since we aren't doing full setup
  auto sink = std::make_shared<MockSink>();
  Logger::set_sink(sink);

  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  gw->initialize(render_system, data_manager);

  std::vector<sb::ecs::Entity> expected_entities = {
    {1, 0}, {2, 0}, {3, 0}, {4, 0},
    {5, 0}, {6, 0}
  };

  EXPECT_CALL(*em, create_entity())
  .Times(AtLeast(4))
  .WillRepeatedly(Return(Entity{99, 0}));

  EXPECT_CALL(*em, set_signature(_, _)).Times(AnyNumber());
  EXPECT_CALL(*em, get_signature(_)).WillRepeatedly(Return(Signature{}));

  Signature renderable_sig;
  renderable_sig.set(cm->get_component_type<RenderableSimpleShape>());
  renderable_sig.set(cm->get_component_type<Transform>());

  EXPECT_CALL(*em, get_entities_with_signature(Eq(renderable_sig)))
    .WillOnce(Return(expected_entities));

  gw->load_level(data_manager->get_current_level_data());

  auto result = gw->get_entities_with_signature(renderable_sig);
  EXPECT_EQ(result.size(), expected_entities.size());
}

TEST(GameWorldTest, UnloadLevelClearsAllEntities) {
  auto em = std::make_shared<MockEntityManager_GMock>();
  auto cm = std::make_shared<ComponentManager>();
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();
  GameWorld gw(em, cm, sm, cq);

  EXPECT_CALL(*em, clear_all()).Times(1);
  gw.unload_level();
}