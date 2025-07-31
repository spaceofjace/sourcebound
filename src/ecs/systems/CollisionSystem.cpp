//
// Created by Jace Shultz on 7/30/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../../include/ecs/systems/CollisionSystem.h"

void sb::ecs::CollisionSystem::update(float /*delta_time*/, ComponentManager& component_manager) {
  std::vector<Entity> clampable_entities;
  std::vector<Entity> clamp_bounds;

  for (const auto& entity : entities) {
    const auto& collider = component_manager.get_component<BoxCollider>(entity);
    if ((collider.behavior & CollisionBehavior::Clamp) == CollisionBehavior::Clamp) {
      clamp_bounds.push_back(entity);  // Static boundary entity
    }

    const bool is_clampable =
      component_manager.has_component<Direction>(entity) &&
      (component_manager.has_component<Paddle>(entity) ||
       component_manager.has_component<StuckToPaddle>(entity));

    if (is_clampable) {
      clampable_entities.push_back(entity);
    }
  }

  for (const auto& entity : clampable_entities) {
    auto& transform = component_manager.get_component<Transform>(entity);
    const auto& collider = component_manager.get_component<BoxCollider>(entity);
    const auto& direction = component_manager.get_component<Direction>(entity);

    for (const auto& boundary : clamp_bounds) {
      if (boundary == entity) { continue; }

      const auto& boundary_transform = component_manager.get_component<Transform>(boundary);
      const auto& boundary_collider = component_manager.get_component<BoxCollider>(boundary);

      if (!has_aabb_overlap(collider, transform, boundary_collider, boundary_transform)){
        continue;
      }

      const float entity_center = transform.position.x + collider.offset_x;
      const float boundary_center = boundary_transform.position.x + boundary_collider.offset_x;

      const float total_half_width = ((collider.width / 2.0F) + (boundary_collider.width / 2.0F));
      const float center_dist = std::abs(entity_center - boundary_center);
      const float overlap = total_half_width - center_dist;

      if (overlap > 0.0F && direction.x != 0.0F) {
        const float correction_dir = (direction.x > 0.0F) ? -1.0F : 1.0F;
        transform.position.x += correction_dir * overlap;
      }

      const float entity_y = transform.position.y + collider.offset_y;
      const float boundary_y = boundary_transform.position.y + boundary_collider.offset_y;
      const float vertical_half = ((collider.height / 2.0F) + (boundary_collider.height / 2.0F));
      const float vertical_dist = std::abs(entity_y - boundary_y);
      const float y_overlap = vertical_half - vertical_dist;

      if (y_overlap > 0.0F && direction.y != 0.0f) {
        const float y_correction = (direction.y > 0.0f) ? -1.0f : 1.0f;
        transform.position.y += y_correction * y_overlap;
      }
    }
  }
}

bool sb::ecs::CollisionSystem::has_aabb_overlap(const BoxCollider& box_a, const Transform& transform_a,
  const BoxCollider& box_b, const Transform& transform_b) {

  const float a_left   = transform_a.position.x + box_a.offset_x - (box_a.width  / 2.0F);
  const float a_right  = transform_a.position.x + box_a.offset_x + (box_a.width  / 2.0F);
  const float a_top    = transform_a.position.y + box_a.offset_y - (box_a.height / 2.0F);
  const float a_bottom = transform_a.position.y + box_a.offset_y + (box_a.height / 2.0F);

  const float b_left   = transform_b.position.x + box_b.offset_x - (box_b.width  / 2.0F);
  const float b_right  = transform_b.position.x + box_b.offset_x + (box_b.width  / 2.0F);
  const float b_top    = transform_b.position.y + box_b.offset_y - (box_b.height / 2.0F);
  const float b_bottom = transform_b.position.y + box_b.offset_y + (box_b.height / 2.0F);

  return a_right >= b_left && a_left <= b_right && a_bottom >= b_top && a_top <= b_bottom;
}