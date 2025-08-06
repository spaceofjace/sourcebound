//
// Created by Jace Shultz on 7/31/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../../include/ecs/systems/FollowSystem.h"
#include "../../../include/ecs/components/Components.h"
#include "../../../include/gamestate/GameWorld.h"

void sb::ecs::FollowSystem::update(float /*delta_time*/, gamestate::GameWorld&  game_world) {
  std::vector snapshot(entities.begin(), entities.end());
  for (const Entity& entity : snapshot) {

    auto& transform = game_world.get_component<Transform>(entity);
    const auto& follower = game_world.get_component<PositionFollower>(entity);

    if (!game_world.has_component<Transform>(follower.target)) {
      log::Logger::warn("[FollowSystem]: Target entity does not have a transform component.");
      continue;
    }

    const auto& target_transform = game_world.get_component<Transform>(follower.target);
    transform.position.x = target_transform.position.x + follower.offset.x;
    transform.position.y = target_transform.position.y + follower.offset.y;
  }
}