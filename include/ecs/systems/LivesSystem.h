//
// Created by Jace Shultz on 8/6/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
* @file LivesSystem.h
* @ingroup ECS
* @brief Tracks ball entity presence to manage player lives and trigger stage transitions.
*
* @author Jace Shultz
* @date 8/6/2025
*/
#ifndef LIVESSYSTEM_H
#define LIVESSYSTEM_H

#include <memory>
#include "../ISystem.h"
#include "../../data/IGameDataManager.h"

namespace sb::ecs {
/**
* @class LivesSystem
* @brief Checks for loss of all Ball entities and manages life count.
*
* This system monitors whether any active `Ball` entities exist in the world; it's currently an
* assumption that if all balls are lost during gameplay, the player has "lost" it decrements the
* player's lives and transitions the stage lifecycle state accordingly.
*/
class LivesSystem final : public ISystem {
public:
  explicit LivesSystem(std::shared_ptr<data::IGameDataManager> game_data_manager)
      : game_data_manager_(std::move(game_data_manager)) {}

  /**
  * @brief Checks for loss of all Ball entities and manages life count.
  * @param delta_time Unused in this system (included for interface compatibility)
  * @param game_world The game world facade providing access to components
  */
  void update(float delta_time, gamestate::GameWorld& game_world) override;

private:
  std::shared_ptr<data::IGameDataManager> game_data_manager_;
};
}  // namespace sb::ecs
#endif //LIVESSYSTEM_H
