//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef ISYSTEM_H
#define ISYSTEM_H
#include "Entity.h"
#include <unordered_set>
namespace sb::ecs {

class ISystem {
public:
  virtual ~ISystem() = default;
  ISystem() = default;

  ISystem(const ISystem&) = delete;
  ISystem& operator=(const ISystem&) = delete;
  ISystem(ISystem&&) = delete;
  ISystem& operator=(ISystem&&) = delete;

  std::unordered_set<Entity> entities;
};

} // namespace sb::ecs
#endif //ISYSTEM_H
