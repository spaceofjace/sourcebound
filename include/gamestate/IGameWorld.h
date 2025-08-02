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

#include "../data/IGameDataManager.h"
#include "../data/LevelData.h"
#include "../ecs/Entity.h"
#include "../ecs/ISystem.h"

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

  /**
   * @brief Initializes game world systems and registers necessary component types.
   * @param renderSystem Rendering has an external dependency, so we are passing it through directly
   * for now.
   *
   * Note: Eventually this param may be replaced with the concept of a "Context" or
   * something very similar.
   */
  virtual void initialize(std::shared_ptr<ecs::ISystem> render_system,
    std::shared_ptr<data::IGameDataManager> game_data_manager) = 0;

  /**
   * @brief Creates a new entity within the game world.
   * @return The newly created entity.
   */
  virtual Entity create_entity() = 0;

  /**
   * @brief Retrieves all entities that match the given signature.
   * @param target_signature A bitmask representing required components.
   * @return A list of matching entities.
   */
  [[nodiscard]] virtual std::vector<Entity> get_entities_with_signature(
      const ecs::Signature& target_signature) const = 0;

  /**
   * @brief Destroys the specified entity and removes its associated components.
   * @param entity The entity to destroy.
   */
  virtual void destroy_entity(Entity entity) = 0;

  /**
   * @brief Executes a full simulation step: processes input commands and updates systems.
   * @param delta_time Elapsed time since the last frame, in seconds.
   */
  virtual void step(float delta_time) = 0;

  /**
   * @brief Updates all registered systems. Typically called once per frame.
   * @param delta_time Elapsed time since the last frame, in seconds.
   */
  virtual void update(float delta_time) = 0;

  /**
   * @brief Processes game world events. Placeholder for future event system integration.
   */
  virtual void process_events() = 0;

  /**
   * @brief Checks if the specified entity is still valid and alive.
   * @param entity The entity to verify.
   * @return True if the entity is active, false if it has been destroyed.
   */
  [[nodiscard]] virtual bool is_alive(Entity entity) const = 0;

  /**
   * @brief Returns true if the world has been flagged for exit (e.g., shutdown or quit).
   * @return True if exit has been requested.
   */
  [[nodiscard]] virtual bool should_exit() const = 0;

  /**
   * @brief Signals that the world should shut down on the next frame.
   */
  virtual void request_exit() = 0;

  /**
   * @brief Loads a level using the provided layout data.
   * @param level_data Game-specific level dimensions and entity parameters.
   */
  virtual void load_level(const data::LevelData& level_data) = 0;

  /**
   * @brief Destroys all active entities and resets the world to a clean state.
   */
  virtual void unload_level() = 0;
};

} // namespace sb::ecs
#endif //IGAMEWORLD_H
