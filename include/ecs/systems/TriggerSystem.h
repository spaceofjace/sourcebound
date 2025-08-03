//
// Created by Jace Shultz on 8/1/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
* @file TriggerSystem.h
* @ingroup ECS
* @brief Processes trigger interactions between entities and emits effect components.
*
* @author Jace Shultz
* @date 8/1/2025
*/
#ifndef TRIGGERSYSTEM_H
#define TRIGGERSYSTEM_H

#include <memory>
#include "../ISystem.h"
#include "../../data/IGameDataManager.h"
#include "../IComponentManager.h"

namespace sb::ecs {

/**
* @class TriggerSystem
* @ingroup ECS
* @brief Scans for triggered entities and applies logic based on Triggerable and WasTriggered.
*
* Entities with a WasTriggered component will be checked for any associated Triggerable types
* and may receive new effect components (e.g., PendingHit, PendingDestroy) as a result.
*
* Trigger behavior is modular — multiple TriggerType values may be present, allowing a single
* trigger to produce multiple effects in downstream systems.
*
* Requires both WasTriggered and Triggerable components on the same entity.
*/
class TriggerSystem final : public ISystem {
public:
  explicit TriggerSystem(std::shared_ptr<data::IGameDataManager> game_data_manager)
      : game_data_manager_(std::move(game_data_manager)) {}

  /**
  * @brief Checks for WasTriggered and forwards on to other systems, as needed.
  * @param delta_time Unused in this system (included for interface compatibility)
  * @param game_world The game world facade providing access to components
  */
  void update(float delta_time, gamestate::GameWorld& game_world) override;

private:
  std::shared_ptr<data::IGameDataManager> game_data_manager_;
};
}  // namespace sb::ecs
#endif //TRIGGERSYSTEM_H
