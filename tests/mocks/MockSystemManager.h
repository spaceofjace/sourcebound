//
// Created by Jace Shultz on 5/8/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef MOCKSYSTEMMANAGER_H
#define MOCKSYSTEMMANAGER_H

#include "../../include/ecs/ISystemManager.h"

using namespace sb::ecs;

struct MockSystemManager : ISystemManager {
  void register_system(std::type_index type, std::shared_ptr<ISystem> system) override{}
  void set_signature(std::type_index type, Signature signature) override{}
  [[nodiscard]] std::shared_ptr<ISystem> get_system(std::type_index type) const override {
    return nullptr;
  }
  void update_all(float /*delta_time*/, ComponentManager& /*component_manager*/) override {}
  void entity_destroyed(Entity entity) override{}
  void entity_signature_changed(Entity entity, const Signature& signature) override{}
};
#endif //MOCKSYSTEMMANAGER_H
