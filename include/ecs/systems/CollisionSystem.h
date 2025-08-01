//
// Created by Jace Shultz on 7/30/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
* @file CollisionSystem.h
 * @ingroup ECS
 * @brief Resolves entity collisions
 *
 * Requires:
 * - Transform and Direction on the moving entity
 * - BoxCollider or CircleCollider as needed
 *
 * @author Jace Shultz
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
 * @ingroup ECS
 * @brief Processes collisions between entities using physics-inspired rules and ECS-friendly behavior markers.
 *
 * This system supports modular collision logic between entities using AABB or Circle colliders.
 *
 * Behaviors Supported:
 * - `Clamp` — Prevents movement outside bounds by repositioning entities (e.g., paddle vs wall)
 * - `Bounce` — Reflects Direction based on collision normal, including circle-to-box logic
 * - `PositionBasedBounce` — Optional tag that alters bounce angle based on hit position (e.g., classic paddle curve)
 *
 * Not Yet Implemented:
 * - `Destroy` — Intended for block destruction or entity elimination on impact
 * - `Trigger` — Future hook for signaling game events (e.g., pickups, portals)
 */


class CollisionSystem final : public ISystem {
 public:
  explicit CollisionSystem(std::shared_ptr<data::IGameDataManager> game_data_manager)
      : game_data_manager_(std::move(game_data_manager)) {}

  /**
  * @brief Performs collision detection and applies movement constraints or bounce behaviors, etc.
  * @param delta_time Frame time (unused)
  * @param component_manager Accessor to entity components
  */
  void update(float delta_time, ComponentManager& component_manager) override;

private:
  /**
  * @brief Checks if two AABB-aligned boxes overlap.
  */
  static bool has_aabb_overlap(const BoxCollider& box_a, const Transform& transform_a,
    const BoxCollider& box_b, const Transform& transform_b);

  /**
  * @brief Checks if a circle overlaps with an AABB box.
  */
  static bool has_circle_aabb_overlap(const CircleCollider& circle, const Transform& circle_transform,
    const BoxCollider& box, const Transform& box_transform);

  /**
  * @brief Computes a surface normal vector for the point of collision between a circle and a box.
  */
  static math::Vec2 get_collision_normal(const math::Vec2& circle_center, const BoxCollider& box,
    const Transform& box_transform);

  std::shared_ptr<data::IGameDataManager> game_data_manager_;
};
} // namespace sb::ecs
#endif //COLLISIONSYSTEM_H
