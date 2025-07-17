//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file SystemManager.h
 * @ingroup ECS
 * @brief Manages ECS systems and their component signatures.
 *
 * Handles registration, signature assignment, and update control for all ECS systems.
 * Uses AND-matching for component signatures. OR conditions must be handled manually
 * in the system's Update method.
 *
 * @author Jace Shultz
 * @date 4/22/2025
 */

#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <memory>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

#include "../logger/sinks/ConsoleSink.h"
#include "Entity.h"
#include "ISystem.h"
#include "ISystemManager.h"
#include "ecs_config.h"

namespace sb::ecs {
/**
 * @class SystemManager
 * @ingroup ECS
 * @brief Manages the lifecycle and controls execution order of ECS systems.
 */

class SystemManager final : public ISystemManager {
public:
  SystemManager() = default;

/**
 * @brief Removes the specified entity from all system tracking.
 *
 * Called automatically when an entity is destroyed.
 *
 * @param entity The entity being destroyed.
 */
  void entity_destroyed(Entity entity) override;

/**
 * @brief Updates entity inclusion in systems based on signature change.
 *
 * Called whenever an entity's component mask is modified.
 *
 * @param entity The modified entity.
 * @param signature The updated component signature.
 */
  void entity_signature_changed(Entity entity, const Signature& signature) override;

/**
 * @brief Calls update on all registered systems.
 *
 * Each system may respond to delta time independently.
 *
 * @param delta_time Time passed since the last update.
 */
  void update(float delta_time) override;

/**
 * @brief Registers a new ECS system.
 *
 * Accepts constructor arguments to support dependency injection.
 *
 * @tparam T The type of the system to register.
 * @tparam Args Types of arguments to forward.
 * @param args Constructor arguments for the system.
 * @return Shared pointer to the newly registered system.
 */
  template <typename T, typename... Args>
  std::shared_ptr<T> register_system(Args&&... args) {
    const std::type_index type = typeid(T);
    if (systems_.find(type) != systems_.end()) {
      log::Logger::error("[SystemManager] System already registered");
      throw std::runtime_error("[SystemManager] System already registered.");
    }

    auto system = std::make_shared<T>(std::forward<Args>(args)...);
    systems_[type] = system;
    return system;
  }

/**
 * @brief Assigns the required component signature for a system.
 *
 * Signature matching uses strict AND logic — all specified components
 * must be present on an entity to be matched. For OR behavior, systems
 * must manually check component presence inside their Update() method.
 *
 * @tparam T The system type.
 * @param signature Bitmask of required components.
 */
  template <typename T>
  void set_signature(Signature signature) {
    if (systems_.find(typeid(T)) == systems_.end()) {
      log::Logger::error("[SystemManager] System not registered before setting signature.");
      throw std::runtime_error("[SystemManager] System not registered before setting signature.");
    }
    const std::type_index type = typeid(T);
    signatures_[type] = signature;
  }

/**
   * @brief Retrieves a registered system by type.
   *
   * @tparam T The system type.
   * @return Shared pointer to the system.
   */
  template <typename T>
  std::shared_ptr<T> get_system() {
    const std::type_index type = typeid(T);
    if (systems_.find(type) == systems_.end()) {
      log::Logger::error("[SystemManager] System not registered.");
      throw std::runtime_error("[SystemManager] System not registered.");
    }
    return std::static_pointer_cast<T>(systems_[type]);
  }

private:
  std::unordered_map<std::type_index, Signature> signatures_;
  std::unordered_map<std::type_index, std::shared_ptr<ISystem>> systems_;
};

} // namespace sb::ecs

#endif //SYSTEMMANAGER_H
