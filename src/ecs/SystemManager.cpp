//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/ecs/SystemManager.h"

namespace sb::ecs {

void SystemManager::entity_destroyed(Entity entity) {
  for (auto& [_, system] : systems_) {
    system->entities.erase(entity);
  }
}

void SystemManager::entity_signature_changed(const Entity entity, const Signature& signature) {
  for (auto& [type, system] : systems_) {
    const Signature& required = signatures_[type];
    if ((signature & required) == required) {
      system->entities.insert(entity);
    } else {
      system->entities.erase(entity);
    }
  }
}

void SystemManager::update_all(const float delta_time, ComponentManager& component_manager) {
  for (const auto& [type, system] : systems_) {
    system->update(delta_time, component_manager);
  }
}

void SystemManager::register_system(const std::type_index type, std::shared_ptr<ISystem> system) {
  if (systems_.find(type) != systems_.end()) {
    log::Logger::error("[SystemManager] System already registered");
    throw std::runtime_error("[SystemManager] System already registered.");
  }
  systems_[type] = std::move(system);
}

void SystemManager::set_signature(const std::type_index type, const Signature signature) {
  if (systems_.find(type) == systems_.end()) {
    log::Logger::error("[SystemManager] System not registered before setting signature.");
    throw std::runtime_error("[SystemManager] System not registered before setting signature.");
  }
  signatures_[type] = signature;
}

std::shared_ptr<ISystem> SystemManager::get_system(const std::type_index type) const {
  auto it = systems_.find(type);
  if (it == systems_.end()) {
    log::Logger::error("[SystemManager] System not registered.");
    throw std::runtime_error("[SystemManager] System not registered.");
  }
  return it->second;
}

} // namespace sb::ecs