//
// Created by Jace Shultz on 5/8/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef MOCKCOMPONENTMANAGER_H
#define MOCKCOMPONENTMANAGER_H

#include <unordered_map>
#include "../../include/ecs/IComponentManager.h"

using namespace sb::ecs;

struct DummyComponent {
  int value{};
};


struct MockComponentManager : IComponentManager {
  std::unordered_map<EntityId, DummyComponent> components;
  ComponentType dummy_type_id = 1;

  template <typename T>
  void add_component(const Entity entity, const T& component) {
    components[entity.id] = static_cast<DummyComponent>(component);
  }

  template <typename T>
  void remove_component(const Entity entity) {
    components.erase(entity.id);
  }

  template <typename T>
  T& get_component(const Entity entity) {
    return reinterpret_cast<T&>(components.at(entity.id));
  }

  template <typename T>
  [[nodiscard]] bool has_component(Entity entity) const {
    return components.find(entity.id) != components.end();
  }

  template <typename T>
  [[nodiscard]] ComponentType get_component_type() const {
    return dummy_type_id;
  }
  void entity_destroyed(Entity entity) override {}
};
#endif //MOCKCOMPONENTMANAGER_H
