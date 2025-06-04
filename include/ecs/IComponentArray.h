//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file IComponentArray.h
 * @ingroup ECS
 * @brief A data structure built for managing a specific component type
 */

#ifndef ICOMPONENTARRAY_H
#define ICOMPONENTARRAY_H
#include "Entity.h"

namespace sb::ecs {

class IComponentArray {
public:
  virtual ~IComponentArray() = default;

  IComponentArray() = default;
  IComponentArray(const IComponentArray&) = delete;
  IComponentArray& operator=(const IComponentArray&) = delete;
  IComponentArray(IComponentArray&&) = delete;
  IComponentArray& operator=(IComponentArray&&) = delete;

  virtual void entity_destroyed(Entity entity) = 0;
};

} // namespace sb::ecs
#endif //ICOMPONENTARRAY_H
