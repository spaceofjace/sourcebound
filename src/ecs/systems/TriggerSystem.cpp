//
// Created by Jace Shultz on 8/1/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../../include/ecs/systems/TriggerSystem.h"
#include "../../../include/ecs/components/Components.h"
#include "../../../include/gamestate/GameWorld.h"

void sb::ecs::TriggerSystem::update(float /*delta_time*/, gamestate::GameWorld&  game_world) {
  std::vector snapshot(entities.begin(), entities.end());
  for (const Entity& entity : snapshot) {
    if (!game_world.has_component<Triggerable>(entity) || !game_world.has_component<WasTriggered>(entity)) {
      log::Logger::warn("[TriggerSystem]: Entity does not have a triggerable or was triggered component.");
      continue;  // should never happen due to signature
    }

    const auto& triggers = game_world.get_component<Triggerable>(entity);
    const auto& was_triggered = game_world.get_component<WasTriggered>(entity);

    for (auto trigger_type : triggers.types) {
      switch (trigger_type) {
        case TriggerType::Hit:
          if (!game_world.has_component<PendingHit>(entity)) {
            game_world.add_component<PendingHit>(entity, PendingHit{1});
          }
        break;

        case TriggerType::Destroy:
          if (!game_world.has_component<PendingDestroy>(was_triggered.source)) {
            game_world.add_component<PendingDestroy>(was_triggered.source, {});
          }
        break;

        case TriggerType::None:
          default:
            break;
      }
    }

    game_world.remove_component<WasTriggered>(entity);
  }
}