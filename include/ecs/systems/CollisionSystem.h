//
// Created by Jace Shultz on 7/30/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file CollisionSystem.h
 * @ingroup [group]
 * @brief [brief description of file's purpose]
 * 
 * @author shult
 * @date 7/30/2025
 */
#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include <memory>
#include "../components/Components.h"
#include "../../data/IGameDataManager.h"
#include "../IComponentManager.h"
#include "../ISystem.h"

namespace sb::ecs {

/**
 * @class CollisionSystem
 * @brief
*/


class CollisionSystem final : public ISystem {
 public:
  explicit CollisionSystem(std::shared_ptr<data::IGameDataManager> game_data_manager)
      : game_data_manager_(std::move(game_data_manager)) {}

  void update(float delta_time, ComponentManager& component_manager) override;

private:
  static bool has_aabb_overlap(const BoxCollider& box_a, const Transform& transform_a,
    const BoxCollider& box_b, const Transform& transform_b);
  std::shared_ptr<data::IGameDataManager> game_data_manager_;
};
} // namespace sb::ecs
#endif //COLLISIONSYSTEM_H
