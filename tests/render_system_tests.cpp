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
#include "mocks/MockRenderer.h" // GMock version
#include "mocks/MockSink.h"

using namespace sb::ecs;
using namespace sb::rendering;
using ::testing::_;
using ::testing::Eq;
using ::testing::ByRef;
using ::testing::Return;

TEST(RenderSystemTest, RendersCircleShapeCorrectly) {
  auto mock_renderer = std::make_shared<MockRenderer>();
  auto real_component_mgr = std::make_shared<ComponentManager>();

  RenderSystem render_system(mock_renderer, real_component_mgr);

  Entity entity{1, 0};
  render_system.entities.insert(entity);

  Transform transform = Transform{{100.f, 200.f}, {}, {25.f, 25.f}, {}};
  RenderableSimpleShape shape = RenderableSimpleShape{ Colors::red, SimpleShapeType::Circle, true};

  real_component_mgr->register_component<Transform>();
  real_component_mgr->register_component<RenderableSimpleShape>();

  real_component_mgr->add_component<Transform>(entity, transform);
  real_component_mgr->add_component<RenderableSimpleShape>(entity, shape);


  EXPECT_CALL(*mock_renderer,
    draw_circle(
      Eq(static_cast<int>(transform.position.x)),
      Eq(static_cast<int>(transform.position.y)),
      Eq(static_cast<int>(transform.size.width * 0.5f)), //halves in production code
      Eq(Colors::red),
      Eq(true)))
    .Times(1);

  render_system.update(0.0f);
}

TEST(RenderSystemTest, RendersRectangleShapeCorrectly) {
  auto mock_renderer = std::make_shared<MockRenderer>();
  auto real_component_mgr = std::make_shared<ComponentManager>();

  RenderSystem render_system(mock_renderer, real_component_mgr);

  Entity entity{2, 0};
  render_system.entities.insert(entity);

  Transform transform = Transform{{10.f, 20.f}, {}, {60.f, 30.f}, {}};
  RenderableSimpleShape shape = RenderableSimpleShape{Colors::cyan, SimpleShapeType::Rectangle, false};

  real_component_mgr->register_component<Transform>();
  real_component_mgr->register_component<RenderableSimpleShape>();

  real_component_mgr->add_component<Transform>(entity, transform);
  real_component_mgr->add_component<RenderableSimpleShape>(entity, shape);


  EXPECT_CALL(*mock_renderer,
    draw_rect(
      static_cast<int>(transform.position.x),
      static_cast<int>(transform.position.y),
      static_cast<int>(transform.size.width),
      static_cast<int>(transform.size.height),
      shape.color,
      shape.filled))
    .Times(1);

  render_system.update(0.0f);
}

TEST(RenderSystemTest, LogsErrorOnInvalidShapeType) {
  auto mock_sink = std::make_shared<MockSink>();
  Logger::set_sink(mock_sink);

  auto mock_renderer = std::make_shared<MockRenderer>();
  auto real_component_mgr = std::make_shared<ComponentManager>();

  RenderSystem render_system(mock_renderer, real_component_mgr);

  Entity entity{3, 0};
  render_system.entities.insert(entity);

  auto transform = Transform{{10.f, 10.f}, {}, {10.f, 10.f}, {}};
  auto shape = RenderableSimpleShape {Colors::dark_gray, SimpleShapeType::Invalid, true};

  real_component_mgr->register_component<Transform>();
  real_component_mgr->register_component<RenderableSimpleShape>();

  real_component_mgr->add_component<Transform>(entity, transform);
  real_component_mgr->add_component<RenderableSimpleShape>(entity, shape);

  // No EXPECT_CALL since we only log — but this shouldn't crash
  render_system.update(0.0f);

  EXPECT_EQ(mock_sink->last_level, sb::log::Level::Error);
  EXPECT_EQ(mock_sink->last_message,"Unsupported shape type detected for entity ID: " +
    std::to_string(entity.id) + "");
}