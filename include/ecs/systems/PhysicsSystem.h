//
// Created by Jace Shultz on 7/25/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file PhysicsSystem.h
 * @ingroup [group]
 * @brief [brief description of file's purpose]
 * 
 * @author shult
 * @date 7/25/2025
 */
#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <memory>
#include "../ISystem.h"
#include "../../data/IGameDataManager.h"
#include "../IComponentManager.h"

namespace sb::ecs {

/**
 * @class PhysicsSystem
 * @brief Applies movement to entities based on velocity and configuration data.
 *
 * Reads movement scalar from Velocity, applies frame-adjusted translation to Transform.
 * Uses current LevelData from IGameDataManager to determine pixel speed.
 */
class PhysicsSystem final : public ISystem {
public:
  explicit PhysicsSystem(std::shared_ptr<data::IGameDataManager> game_data_manager)
      : game_data_manager_(std::move(game_data_manager)) {}

  void update(float delta_time, gamestate::GameWorld& game_world) override;

private:
  std::shared_ptr<data::IGameDataManager> game_data_manager_;
};
} // namespace sb::ecs
#endif //PHYSICSSYSTEM_H
