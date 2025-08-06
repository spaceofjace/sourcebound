//
// Created by Jace Shultz on 8/2/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
* @file HitSystem.h
* @ingroup ECS
* @brief Applies damage logic by reducing HitsRequired when a PendingHit is present.
*
* @author Jace Shultz
* @date 8/2/2025
*/
#ifndef HITSYSTEM_H
#define HITSYSTEM_H
#include <memory>
#include "../ISystem.h"
#include "../../data/IGameDataManager.h"
#include "../IComponentManager.h"

namespace sb::ecs {

/**
* @class HitSystem
* @ingroup ECS
* @brief Processes PendingHit and decrements entity durability, marking for destruction if depleted.
*
* Entities with a PendingHit will have their HitsRequired reduced. If the value reaches zero or below,
* the entity is marked with a PendingDestroy component, allowing a subsequent DestructionSystem to remove it.
*
* Also supports future extensions like score updates, sound triggers, and visual feedback.
*
* Requires both PendingHit and HitsRequired on the same entity.
*/
class HitSystem final : public ISystem {
public:
  explicit HitSystem(std::shared_ptr<data::IGameDataManager> game_data_manager)
      : game_data_manager_(std::move(game_data_manager)) {}

  /**
  * @brief Checks for pending hits and applies them as appropriate
  * @param delta_time Unused in this system (included for interface compatibility)
  * @param game_world The game world facade providing access to components
  */
  void update(float delta_time, gamestate::GameWorld& game_world) override;

private:
  std::shared_ptr<data::IGameDataManager> game_data_manager_;
};
}  // namespace sb::ecs
#endif //HITSYSTEM_H
