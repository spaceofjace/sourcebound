//
// Created by Jace Shultz on 5/8/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file IGameWorld.h
 * @ingroup gamestate
 * @brief Declares the interface for interacting with the active game world state.
 */

/**
 * @defgroup gamestate Game State
 * @brief Module wrapping the ECS facade and elements that will mutate game state (such as commands)
 *
 * This group includes a facade for centralizing and coordinating ECS behaviors, as well as commands
 * and supporting structures for mutating game state (especially via player input)
 */

#ifndef IGAMEWORLD_H
#define IGAMEWORLD_H

#include "../ecs/Entity.h"

namespace sb::gamestate {
using ecs::Entity;
/**
 * @interface IGameWorld
 * @ingroup gamestate
 * @brief Abstract representation of a game world. Used to interact with ECS layers and runtime data.
 *
 * GameWorld serves two primary purposes here:
 *   1. to centralize entry points for common ECS operations and ensure correct coordination across
 *      modules.
 *   2. to theoretically allow replacement with a more complete, more robust ECS (e.g., flecs) at
 *      some later stage, if desired.
 */
class IGameWorld {
public:
  IGameWorld() = default;
  virtual ~IGameWorld() = default;

  IGameWorld(const IGameWorld&) = delete;
  IGameWorld& operator=(const IGameWorld&) = delete;
  IGameWorld(IGameWorld&&) = delete;
  IGameWorld& operator=(IGameWorld&&) = delete;

  virtual Entity create_entity() = 0;
  [[nodiscard]] virtual std::vector<Entity> get_entities_with_signature(
      const ecs::Signature& target_signature) const = 0;
  virtual void destroy_entity(Entity entity) = 0;
  virtual void step(float delta_time) = 0;
  virtual void update(float delta_time) = 0;
  virtual void process_events() = 0;
  [[nodiscard]] virtual bool is_alive(Entity entity) const = 0;
};

} // namespace sb::ecs
#endif //IGAMEWORLD_H
