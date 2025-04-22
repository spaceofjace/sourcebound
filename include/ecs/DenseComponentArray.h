//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef DENSECOMPONENTARRAY_H
#define DENSECOMPONENTARRAY_H

#include "IComponentArray.h"
#include <unordered_map>

// Staying simple with dense array for now, but will hopefully be able to switch to be sparse later
template <typename T>
class DenseComponentArray final : public IComponentArray {
  public:
  void insert_data(Entity entity, const T& component) {
    entity_to_component_[entity] = component;
  }

  void remove_data(Entity entity) {
    entity_to_component_.erase(entity);
  }

  T& get_data(Entity entity) {
    return entity_to_component_[entity];
  }

  [[nodiscard]] bool has_data(Entity entity) const {
    return entity_to_component_.find(entity) != entity_to_component_.end();
  }

  void entity_destroyed(const Entity entity) override {
    remove_data(entity);
  }

  private:
  std::unordered_map<Entity, T> entity_to_component_;
};

#endif //DENSECOMPONENTARRAY_H
