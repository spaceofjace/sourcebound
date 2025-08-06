//
// Created by Jace Shultz on 8/2/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../../include/ecs/systems/DestructionSystem.h"
#include "../../../include/gamestate/GameWorld.h"

void sb::ecs::DestructionSystem::update(float /*delta_time*/, gamestate::GameWorld& game_world) {
  std::vector snapshot(entities.begin(), entities.end());
  for (const Entity& entity : snapshot) {
    if (!game_world.has_component<PendingDestroy>(entity)) {
      log::Logger::warn("[DestructionSystem]: Entity is missing PendingDestroy, unexpected for this system.");
      continue;  // should never happen due to signature
    }

    game_world.remove_component<PendingDestroy>(entity);
    if (!game_world.has_component<Indestructible>(entity)) {
      game_world.destroy_entity(entity);
    }
  }
}