//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file DenseComponentArray.h
 * @ingroup ECS
 * @brief Provides dense storage for components of a specific type.
 */

#ifndef DENSECOMPONENTARRAY_H
#define DENSECOMPONENTARRAY_H

#include "IComponentArray.h"
#include <unordered_map>

/**
 * @class DenseComponentArray
 * @ingroup ECS
 * @brief Provides dense storage for components of a specific type.
 *
 * Used by ComponentManager to enable efficient iteration and cache-friendly access.
 * (Can be swapped for a sparse array later)
 */

namespace sb::ecs {
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

} // namespace sb::ecs
#endif //DENSECOMPONENTARRAY_H
