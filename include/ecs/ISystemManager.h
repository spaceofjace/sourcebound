//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file ISystemManager.h
 * @ingroup ECS
 * @brief Declares the interface for managing registered systems.
 */

#ifndef ISYSTEMMANAGER_H
#define ISYSTEMMANAGER_H
#include <memory>
#include <typeindex>
#include "Entity.h"
#include "ecs_config.h"
#include "ISystem.h"

namespace sb::ecs {
/**
 * @interface ISystemManager
 * @ingroup ECS
 * @brief Interface for system registration, lifecycle control, and execution.
 */
class ISystemManager {
public:
  virtual ~ISystemManager() = default;
  ISystemManager() = default;

  ISystemManager(const ISystemManager&) = delete;
  ISystemManager& operator=(const ISystemManager&) = delete;
  ISystemManager(ISystemManager&&) = delete;
  ISystemManager& operator=(ISystemManager&&) = delete;

  /**
  * @brief Calls update on all registered systems.
  *
  * Each system may respond to delta time independently.
  *
  * @param delta_time Time passed since the last update.
  * @param game_world Used to retrieve and modify components
  */
  virtual void update_all(float delta_time, gamestate::GameWorld& game_world) = 0;

  /**
  * @brief Removes the specified entity from all system tracking.
  *
  * Called automatically when an entity is destroyed.
  *
  * @param entity The entity being destroyed.
  */
  virtual void entity_destroyed(Entity entity) = 0;

  /**
  * @brief Updates entity inclusion in systems based on signature change.
  *
  * Called whenever an entity's component mask is modified.
  *
  * @param entity The modified entity.
  * @param signature The updated component signature.
  */
  virtual void entity_signature_changed(Entity entity, const Signature& signature) = 0;

  /**
  * @brief Registers a system instance under a runtime type key.
  *
  * Allows runtime-safe system injection (e.g., for tests or scripting). Must be called before
  * setting a signature or retrieving the system.
  *
  * @param type Unique type index for the system.
  * @param system Shared pointer to the system instance.
  * @throws std::runtime_error if the system type is already registered.
  */
  virtual void register_system(std::type_index type, std::shared_ptr<ISystem> system) = 0;

  /**
  * @brief Assigns a required component signature to a registered system.
  *
  * Uses AND logic: entities must match all required components
  * to be tracked by the system. OR logic must be handled manually
  * in the system’s internal update logic.
  *
  * @param type The runtime type of the system.
  * @param signature The bitmask representing the required components.
  * @throws std::runtime_error if called before system registration.
  */
  virtual void set_signature(std::type_index type, Signature signature) = 0;

  /**
  * @brief Retrieves a registered system by runtime type.
  *
  * @param type The type index of the desired system.
  * @return A shared pointer to the system.
  * @throws std::runtime_error if the system is not registered.
  */
  [[nodiscard]] virtual std::shared_ptr<ISystem> get_system(std::type_index type) const = 0;

  /**
   * @brief Clears all internal system references and resets system state.
   *
   * Implementations should remove all registered systems and any associated metadata.
   */
  virtual void clear_all() = 0;
};

}  // namespace sb::ecs
#endif //ISYSTEMMANAGER_H
