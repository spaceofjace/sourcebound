//
// Created by Jace Shultz on 7/31/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
* @file FollowSystem.h
* @ingroup ECS
* @brief System that causes entities to follow the position of a target entity with a relative offset.
*
* @author Jace Shultz
* @date 7/31/2025
*/
#ifndef FOLLOWSYSTEM_H
#define FOLLOWSYSTEM_H

#include <memory>
#include "../ISystem.h"
#include "../../data/IGameDataManager.h"
#include "../IComponentManager.h"
namespace sb::ecs {

/**
* @class FollowSystem
* @ingroup ECS
* @brief Updates the Transform of entities with PositionFollower to match a target's position + offset.
*
* Entities with a PositionFollower component will adopt the position of their specified target entity,
* adjusted by a fixed offset. This system is ideal for linked movement behaviors such as the ball sticking
* to the paddle or trailing visual elements.
*
* Requires both Transform and PositionFollower components on the follower entity.
* Skips followers if the target entity is not valid or lacks a Transform.
*/
class FollowSystem final : public ISystem {
public:
  /**
  * @brief Constructs the FollowSystem with access to game data (not currently used, but preserved for future use).
  * @param game_data_manager Shared pointer to the IGameDataManager
  */
  explicit FollowSystem(std::shared_ptr<data::IGameDataManager> game_data_manager)
      : game_data_manager_(std::move(game_data_manager)) {}

  /**
  * @brief Updates all follower entities, syncing their position to their target + offset.
  * @param delta_time Unused in this system (included for interface compatibility)
  * @param component_manager The ECS component manager providing access to components
  */
  void update(float delta_time, ComponentManager& component_manager) override;

private:
  std::shared_ptr<data::IGameDataManager> game_data_manager_;
};

} // namespace sb::ecs
#endif //FOLLOWSYSTEM_H
