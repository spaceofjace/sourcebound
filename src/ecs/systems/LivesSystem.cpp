//
// Created by Jace Shultz on 8/6/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../../include/ecs/systems/LivesSystem.h"

#include "../../../include/gamestate/GameWorld.h"
void sb::ecs::LivesSystem::update(float delta_time, gamestate::GameWorld& game_world) {
  if (game_world.get_stage_lifecycle_state() == gamestate::StageLifecycleState::Active) {
    if (entities.empty()) {
      Signature signature;
      signature.set(game_world.get_component_type<PlayerSessionState>());
      auto player_session_entities = game_world.get_entities_with_signature(signature);
      if (player_session_entities.empty()) {
        log::Logger::warn("[LivesSystem]: No player session entities found.");
      }
      else {
        auto& player_data = game_world.get_component<PlayerSessionState>(player_session_entities[0]);
        player_data.lives--;
        if (player_data.lives == 0) {
          game_world.set_stage_lifecycle_state(gamestate::StageLifecycleState::GameOver);
        }
        else {
          game_world.set_stage_lifecycle_state(gamestate::StageLifecycleState::PlayerResetRequested);
        }
      }
    }
  }
}