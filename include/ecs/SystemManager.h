//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file SystemManager.h
 * @ingroup ECS
 * @brief Defines the concrete implementation of ISystemManager.h
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

  void entity_destroyed(Entity entity) override;
  void entity_signature_changed(Entity entity, const Signature& signature) override;
  void update(float delta_time) override;

  template <typename T>
  std::shared_ptr<T> register_system() {
    const std::type_index type = typeid(T);
    if (systems_.find(type) != systems_.end()) {
      log::Logger::error("[SystemManager] System already registered");
      throw std::runtime_error("[SystemManager] System already registered.");
    }

    auto system = std::make_shared<T>();
    systems_[type] = system;
    return system;
  }

  template <typename T>
  void set_signature(Signature signature) {
    if (systems_.find(typeid(T)) == systems_.end()) {
      log::Logger::error("[SystemManager] System not registered before setting signature.");
      throw std::runtime_error("[SystemManager] System not registered before setting signature.");
    }
    const std::type_index type = typeid(T);
    signatures_[type] = signature;
  }

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
