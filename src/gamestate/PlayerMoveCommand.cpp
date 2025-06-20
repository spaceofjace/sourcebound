//
// Created by Jace Shultz on 6/20/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/gamestate/PlayerMoveCommand.h"
#include "../../include/ecs/components/Components.h"

const std::string sb::gamestate::PlayerMoveCommand::name_ = "PlayerMoveCommand";

void sb::gamestate::PlayerMoveCommand::apply(
    std::shared_ptr<GameWorld<IEntityManager, ComponentManager, ISystemManager, ICommandQueue>>
        world) {

  Signature sig;
  sig.set(world->get_component_type<Paddle>());
  sig.set(world->get_component_type<Velocity>());

  auto paddles = world->get_entities_with_signature(sig);
  for (const auto& entity : paddles) {
    auto& velocity = world->get_component<Velocity>(entity);
    velocity.x = x_velocity_;
    velocity.y = y_velocity_;
  }
}