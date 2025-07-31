//
// Created by Jace Shultz on 7/31/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/gamestate/LaunchBallCommand.h"
#include "../../include/ecs/components/Components.h"

using sb::ecs::Ball;
using sb::ecs::Direction;
using sb::ecs::PositionFollower;
using sb::ecs::Transform;

const std::string sb::gamestate::LaunchBallCommand::name_ = "LaunchBallCommand";
void sb::gamestate::LaunchBallCommand::apply(std::shared_ptr<GameWorld> world) {
  Signature sig;
  sig.set(world->get_component_type<Ball>());
  sig.set(world->get_component_type<Direction>());
  sig.set(world->get_component_type<PositionFollower>());
  sig.set(world->get_component_type<Transform>());
  sig.set(world->get_component_type<ecs::CircleCollider>());

  auto balls = world->get_entities_with_signature(sig);
  for (const auto& ball : balls) {
    auto& direction = world->get_component<Direction>(ball);
    direction.x = 0;
    direction.y = -1.0F; //negative is up in the SDL coordinate system

    auto& transform = world->get_component<Transform>(ball);
    auto& circle_collider = world->get_component<ecs::CircleCollider>(ball);
    transform.position.y -= ( circle_collider.radius + circle_collider.offset_y + 1.0F);

    world->remove_component<PositionFollower>(ball);
  }

  log::Logger::info("[Command Applied]: " + name_);
}