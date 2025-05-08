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

void SystemManager::update(const float delta_time) {
  for (const auto& [type, system] : systems_) {
    system->update(delta_time);
  }
}

} // namespace sb::ecs