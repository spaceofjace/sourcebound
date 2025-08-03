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

  void update_all(float delta_time, gamestate::GameWorld& game_world) override;
  void entity_destroyed(Entity entity) override;
  void entity_signature_changed(Entity entity, const Signature& signature) override;
  void register_system(std::type_index type, std::shared_ptr<ISystem> system) override;
  void set_signature(std::type_index type, Signature signature) override;
  [[nodiscard]] std::shared_ptr<ISystem> get_system(std::type_index type) const override;

  /**
   * @brief Templated convenience method to register a system.
   *
   * Forwards constructor arguments and wraps the non-templated
   * register_system using the system's type ID.
   *
   * @tparam T The concrete system type.
   * @tparam Args Constructor argument types.
   * @param args Arguments to pass to the system’s constructor.
   * @return Shared pointer to the registered system.
   */
  template <typename T, typename... Args>
  std::shared_ptr<T> register_system(Args&&... args) {
    const std::type_index type = typeid(T);
    auto system = std::make_shared<T>(std::forward<Args>(args)...);
    register_system(type, system);
    return system;
  }

  /**
   * @brief Templated convenience method to assign a system signature.
   *
   * Calls the non-templated set_signature using the system's type ID.
   *
   * @tparam T The system type.
   * @param signature The required component signature.
   */
  template <typename T>
  void set_signature(const Signature signature) {
    set_signature(typeid(T), signature);
  }

  /**
   * @brief Templated convenience method to retrieve a system.
   *
   * Calls the non-templated get_system and casts to the correct type.
   *
   * @tparam T The desired system type.
   * @return Shared pointer to the system.
   */
  template <typename T>
  std::shared_ptr<T> get_system() {
    return std::static_pointer_cast<T>(get_system(typeid(T)));
  }

private:
  std::unordered_map<std::type_index, Signature> signatures_;
  std::unordered_map<std::type_index, std::shared_ptr<ISystem>> systems_;
};

} // namespace sb::ecs

#endif //SYSTEMMANAGER_H
