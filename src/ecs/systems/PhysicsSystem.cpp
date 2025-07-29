//
// Created by Jace Shultz on 7/25/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//


#include "../../../include/ecs/systems/PhysicsSystem.h"
#include "../../../include/ecs/components/Components.h"

void sb::ecs::PhysicsSystem::update(float delta_time) {
  const auto& level_data = game_data_manager_->get_current_level_data();

  for (const auto& entity : entities) {
    auto& velocity = component_manager_->get_component<Velocity>(entity);
    auto& transform = component_manager_->get_component<Transform>(entity);

    if (component_manager_->has_component<Paddle>(entity)) {
      // Movement comes from input: this is a directional vector
      // will adjust this to be a directional vector and modify velocity separately here
      const float paddle_speed = level_data.paddle_speed;
      transform.position.x += paddle_speed * velocity.x * delta_time;
    } else if (component_manager_->has_component<Ball>(entity)) {
      if(component_manager_->has_component<StuckToPaddle>(entity)){
        // may move this to a Ball Follow System later
        const float paddle_speed = level_data.paddle_speed;
        transform.position.x += paddle_speed * velocity.x * delta_time;
      } else {
        const float ball_speed = level_data.ball_speed;
        transform.position.x += ball_speed * velocity.x * delta_time;
      }
    } else {
      // Skip or log unsupported entity type
    }
  }
}