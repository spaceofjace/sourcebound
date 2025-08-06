//
// Created by Jace Shultz on 8/2/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../../include/ecs/systems/HitSystem.h"
#include "../../../include/gamestate/GameWorld.h"

void sb::ecs::HitSystem::update(float /*delta_time*/, gamestate::GameWorld& game_world) {
  std::vector snapshot(entities.begin(), entities.end());
  for (const Entity& entity : snapshot) {
    if (!game_world.has_component<PendingHit>(entity) || !game_world.has_component<HitsRequired>(entity)) {
      log::Logger::warn("[HitSystem]: Entity missing PendingHit or HitsRequired. Should not occur.");
      continue;  // should never happen due to signature
    }

    auto& hits_required = game_world.get_component<HitsRequired>(entity);
    const auto& pending_hits = game_world.get_component<PendingHit>(entity);

    if (pending_hits.amount >= hits_required.hits) {
      game_world.add_component<PendingDestroy>(entity, {});
    } else {
      hits_required.hits -= pending_hits.amount;;
    }

    game_world.remove_component<PendingHit>(entity);
  }
}