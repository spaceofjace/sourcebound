//
// Created by Jace Shultz on 7/16/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/ecs/RenderSystem.h"

#include <string>

#include "../../include/ecs/components/Components.h"
#include "../../include/logger/sinks/ConsoleSink.h"
#include "../../include/gamestate/GameWorld.h"

void sb::ecs::RenderSystem::update(float /*delta_time*/, gamestate::GameWorld& game_world) {
  std::vector snapshot(entities.begin(), entities.end());
  for (const Entity& entity : snapshot) {

    auto const renderableComponent = game_world.get_component<RenderableSimpleShape>(entity);
    auto const transformComponent = game_world.get_component<Transform>(entity);

    switch (renderableComponent.type) {
      case SimpleShapeType::Rectangle:
        renderer_->draw_rect(transformComponent.position.as_vec2(),
          static_cast<int>(transformComponent.size.width),
          static_cast<int>(transformComponent.size.height),
          renderableComponent.color,
          renderableComponent.filled);
        break;
      case SimpleShapeType::Circle:
        renderer_->draw_circle(transformComponent.position.as_vec2(),
          static_cast<int>(0.5F * transformComponent.size.width), //assume radius from width NOLINT(*-magic-numbers)
          renderableComponent.color,
          renderableComponent.filled);
        break;

      case SimpleShapeType::Invalid:
      default:
        log::Logger::error("Unsupported shape type detected for entity ID: " + std::to_string(entity.id) + "");
        break;
    }
  }
}