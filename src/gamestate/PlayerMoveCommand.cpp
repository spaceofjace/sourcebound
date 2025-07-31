//
// Created by Jace Shultz on 6/20/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/gamestate/PlayerMoveCommand.h"
#include "../../include/ecs/components/Components.h"

const std::string sb::gamestate::PlayerMoveCommand::name_ = "PlayerMoveCommand";

void sb::gamestate::PlayerMoveCommand::apply(
    std::shared_ptr<GameWorld>
        world) {

  Signature sig;
  sig.set(world->get_component_type<Paddle>());
  sig.set(world->get_component_type<Direction>());

  auto paddles = world->get_entities_with_signature(sig);
  for (const auto& entity : paddles) {
    auto& direction = world->get_component<Direction>(entity);
    direction.x = x_direction_;
    direction.y = y_direction_;
  }

  log::Logger::info("[Command Applied]: " + name_ + " - " + std::to_string(x_direction_) + ", " + std::to_string(y_direction_));
}