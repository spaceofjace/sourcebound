//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//
/**
 * @file ISystem.h
 * @ingroup ECS
 * @brief Declares the interface for all game systems in the ECS framework.
 */

#ifndef ISYSTEM_H
#define ISYSTEM_H
#include "Entity.h"
#include <unordered_set>
namespace sb::ecs {

/**
 * @interface ISystem
 * @ingroup ECS
 * @brief Abstract base class for ECS systems. Systems operate on entities and components each frame.
 */
class ISystem {
public:
  virtual ~ISystem() = default;
  ISystem() = default;

  ISystem(const ISystem&) = delete;
  ISystem& operator=(const ISystem&) = delete;
  ISystem(ISystem&&) = delete;
  ISystem& operator=(ISystem&&) = delete;

  virtual void update(float delta_time) = 0;
  std::unordered_set<Entity> entities;
};

} // namespace sb::ecs
#endif //ISYSTEM_H
