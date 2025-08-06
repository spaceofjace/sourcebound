//
// Created by Jace Shultz on 8/2/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../../include/ecs/systems/DestructionSystem.h"
#include "../../../include/gamestate/GameWorld.h"
#include "../../../include/ecs/components/Components.h"

void sb::ecs::DestructionSystem::update(float /*delta_time*/, gamestate::GameWorld& game_world) {
  std::vector snapshot(entities.begin(), entities.end());
  for (const Entity& entity : snapshot) {
    if (!game_world.has_component<PendingDestroy>(entity)) {
      log::Logger::warn("[DestructionSystem]: Entity is missing PendingDestroy, unexpected for this system.");
      continue;  // should never happen due to signature
    }

    game_world.remove_component<PendingDestroy>(entity);
    if (!game_world.has_component<Indestructible>(entity)) {
      //This may need to shift later, but we're assuming if the object destroyed has a ScoreValue
      //component, we should add a pending score event

      if (game_world.has_component<ScoreValue>(entity)) {
        Signature signature;
        signature.set(game_world.get_component_type<PlayerSessionState>());
        auto player_session_entities = game_world.get_entities_with_signature(signature);
        if (player_session_entities.empty()) {
          log::Logger::warn("[LivesSystem]: No player session entities found.");
        }
        else {
          //TODO Formal ScoreSystem? Still thinking more about how I want score managed generally
          auto& player_data = game_world.get_component<PlayerSessionState>(player_session_entities[0]);
          player_data.score += game_world.get_component<ScoreValue>(entity).value;
          log::Logger::info("[DestructionSystem]: Player score increased by " + std::to_string(game_world.get_component<ScoreValue>(entity).value));
          log::Logger::info("[DestructionSystem]: Player score is now " + std::to_string(player_data.score));
        }
      }
      game_world.destroy_entity(entity);
    }
  }
}