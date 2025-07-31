//
// Created by Jace Shultz on 7/31/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/gamestate/LaunchBallCommand.h"
#include "../../include/ecs/components/Components.h"

const std::string sb::gamestate::LaunchBallCommand::name_ = "LaunchBallCommand";
void sb::gamestate::LaunchBallCommand::apply(std::shared_ptr<GameWorld> world) {
  Signature sig;
  sig.set(world->get_component_type<Ball>());
  sig.set(world->get_component_type<Direction>());
  sig.set(world->get_component_type<PositionFollower>());

  auto balls = world->get_entities_with_signature(sig);
  for (const auto& ball : balls) {
    auto& direction = world->get_component<Direction>(ball);
    direction.x = 0;
    direction.y = -1.0F; //negative is up in the SDL coordinate system

    world->remove_component<PositionFollower>(ball);
  }

  log::Logger::info("[Command Applied]: " + name_);
}