//
// Created by Jace Shultz on 7/16/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../include/ecs/Entity.h"
#include "../include/ecs/IComponentManager.h"
#include "../include/ecs/RenderSystem.h"
#include "../include/ecs/components/Components.h"
#include "mocks/MockCommandQueue.h"
#include "mocks/MockEntityManager.h"
#include "mocks/MockGameDataManager.h"
#include "mocks/MockRenderer.h" // GMock version
#include "mocks/MockSink.h"
#include "mocks/MockSystemManager.h"

using namespace sb::ecs;
using namespace sb::rendering;
using ::testing::_;
using ::testing::Eq;
using ::testing::ByRef;
using ::testing::Return;

TEST(RenderSystemTest, RendersCircleShapeCorrectly) {
  auto mock_renderer = std::make_shared<MockRenderer>();

  //unfortunately need concrete version of ComponentManager and GameWorld for now
  auto cm = std::make_shared<ComponentManager>();
  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>(); // can be an empty stub
  auto cq = std::make_shared<MockCommandQueue>(); // not used in test
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  auto render_system = std::make_shared<RenderSystem>(mock_renderer);
  auto gd = std::make_shared<MockGameDataManager>();

  gw->initialize(render_system, gd);

  Entity entity{1, 0};
  render_system->entities.insert(entity);

  Transform transform = Transform{{100.f, 200.f}, {25.f, 25.f}, {}};
  RenderableSimpleShape shape = RenderableSimpleShape{ Colors::red, SimpleShapeType::Circle, true};

  cm->register_component<Transform>();
  cm->register_component<RenderableSimpleShape>();

  cm->add_component<Transform>(entity, transform);
  cm->add_component<RenderableSimpleShape>(entity, shape);


  EXPECT_CALL(*mock_renderer,
    draw_circle(
      Eq(transform.position.as_vec2()),
      Eq(static_cast<int>(transform.size.width * 0.5f)), //halves in production code
      Eq(Colors::red),
      Eq(true)))
    .Times(1);

  render_system->update(0.0f, *gw);
}

TEST(RenderSystemTest, RendersRectangleShapeCorrectly) {
  auto mock_renderer = std::make_shared<MockRenderer>();

  //unfortunately need concrete version of ComponentManager and GameWorld for now
  auto cm = std::make_shared<ComponentManager>();
  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  RenderSystem render_system(mock_renderer);

  Entity entity{2, 0};
  render_system.entities.insert(entity);

  Transform transform = Transform{{10.f, 20.f}, {60.f, 30.f}, {}};
  RenderableSimpleShape shape = RenderableSimpleShape{Colors::cyan, SimpleShapeType::Rectangle, false};

  cm->register_component<Transform>();
  cm->register_component<RenderableSimpleShape>();

  cm->add_component<Transform>(entity, transform);
  cm->add_component<RenderableSimpleShape>(entity, shape);

  EXPECT_CALL(*mock_renderer,
    draw_rect(
      Eq(transform.position.as_vec2()),
      static_cast<int>(transform.size.width),
      static_cast<int>(transform.size.height),
      shape.color,
      shape.filled))
    .Times(1);

  render_system.update(0.0f, *gw);
}

TEST(RenderSystemTest, LogsErrorOnInvalidShapeType) {
  auto mock_sink = std::make_shared<MockSink>();
  Logger::set_sink(mock_sink);

  auto mock_renderer = std::make_shared<MockRenderer>();

  //unfortunately need concrete version of ComponentManager and GameWorld for now
  auto cm = std::make_shared<ComponentManager>();
  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  RenderSystem render_system(mock_renderer);

  Entity entity{3, 0};
  render_system.entities.insert(entity);

  auto transform = Transform{{10.f, 10.f}, {10.f, 10.f}, {}};
  auto shape = RenderableSimpleShape {Colors::dark_gray, SimpleShapeType::Invalid, true};

  cm->register_component<Transform>();
  cm->register_component<RenderableSimpleShape>();

  cm->add_component<Transform>(entity, transform);
  cm->add_component<RenderableSimpleShape>(entity, shape);

  // No EXPECT_CALL since we only log — but this shouldn't crash
  render_system.update(0.0f, *gw);

  EXPECT_EQ(mock_sink->last_level, sb::log::Level::Error);
  EXPECT_EQ(mock_sink->last_message,"Unsupported shape type detected for entity ID: " +
    std::to_string(entity.id) + "");
}