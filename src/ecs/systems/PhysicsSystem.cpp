//
// Created by Jace Shultz on 7/25/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//


#include "../../../include/ecs/systems/PhysicsSystem.h"
#include "../../../include/ecs/components/Components.h"

void sb::ecs::PhysicsSystem::update(float delta_time, ComponentManager& component_manager) {

  const auto& level_data = game_data_manager_->get_current_level_data();

  for (const auto& entity : entities) {
    auto& velocity = component_manager.get_component<Velocity>(entity);
    auto& direction = component_manager.get_component<Direction>(entity);
    auto& transform = component_manager.get_component<Transform>(entity);

    if (component_manager.has_component<Paddle>(entity)) {
      const float paddle_speed = level_data.paddle_speed;
      velocity.x = direction.x * paddle_speed * delta_time;
      velocity.y = direction.y * paddle_speed * delta_time;
      transform.position.x += velocity.x;
      transform.position.y += velocity.y;
    } else if (component_manager.has_component<Ball>(entity)) {
      const float ball_speed = level_data.ball_speed;
      velocity.x = direction.x * ball_speed * delta_time;
      velocity.y = direction.y * ball_speed * delta_time;
      transform.position.x += velocity.x;
      transform.position.y += velocity.y;
    } else {
      // Skip or log unsupported entity type?
    }
  }
}