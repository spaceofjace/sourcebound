//
// Created by Jace Shultz on 8/2/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
* @file DestructionSystem.h
* @ingroup ECS
* @brief Finalizes entity removal for those marked with PendingDestroy unless protected by Indestructible.
*
* @author Jace Shultz
* @date 8/2/2025
*/
#ifndef DESTRUCTIONSYSTEM_H
#define DESTRUCTIONSYSTEM_H
#include <memory>

#include "../../data/IGameDataManager.h"
#include "../../gamestate/IGameWorld.h"
#include "../ISystem.h"

namespace sb::ecs {

/**
* @class DestructionSystem
* @ingroup ECS
* @brief Destroys entities flagged for removal unless they are explicitly marked as Indestructible.
*
* This system checks for PendingDestroy on all entities and removes them from the game world
* unless an Indestructible component is present. It ensures deferred cleanup and allows
* other systems to react before final deletion.
*
* Useful for controlling death animations, logging, and system order guarantees.
*/
class DestructionSystem final : public ISystem {
public:
  explicit DestructionSystem(std::shared_ptr<data::IGameDataManager> game_data_manager)
      : game_data_manager_(std::move(game_data_manager)) {}

  /**
  * @brief Checks for pending destruction and destroys entities as needed.
  * @param delta_time Unused in this system (included for interface compatibility)
  * @param game_world The game world facade providing access to components
  */
  void update(float delta_time, gamestate::GameWorld& game_world) override;

private:
  std::shared_ptr<data::IGameDataManager> game_data_manager_;
};
}  // namespace sb::ecs
#endif //DESTRUCTIONSYSTEM_H
