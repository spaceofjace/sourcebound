//
// Created by Jace Shultz on 7/30/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <algorithm>
#include "../../../include/ecs/systems/CollisionSystem.h"
#include "../../../include/math/MathConsts.h"

void sb::ecs::CollisionSystem::update(float /*delta_time*/, ComponentManager& component_manager) {
  std::vector<Entity> clampables;
  std::vector<Entity> clamp_bounds;

  std::vector<Entity> bounceables;

  for (const Entity& entity : entities) {
    if (!(component_manager.has_component<BoxCollider>(entity)
      || !component_manager.has_component<CircleCollider>(entity))) { continue; }

    const auto& box = component_manager.get_component<BoxCollider>(entity);

    if ((box.behavior & CollisionBehavior::Bounce) == CollisionBehavior::Bounce) {
      bounceables.push_back(entity);
    }

    if ((box.behavior & CollisionBehavior::Clamp) == CollisionBehavior::Clamp) {
      clamp_bounds.push_back(entity);
    }

    const bool is_clampable =
      component_manager.has_component<Direction>(entity) &&
      component_manager.has_component<Paddle>(entity);

    if (is_clampable) {
      clampables.push_back(entity);
    }
  }

  for (const Entity& entity : entities) {
    if (!component_manager.has_component<Transform>(entity) ||
        !component_manager.has_component<Direction>(entity) ||
        !component_manager.has_component<CircleCollider>(entity)) { continue; }

    auto& transform = component_manager.get_component<Transform>(entity);
    auto& direction = component_manager.get_component<Direction>(entity);
    const auto& circle = component_manager.get_component<CircleCollider>(entity);

    const math::Vec2 circle_center =
        (transform.position + sb::math::Vec2{circle.offset_x, circle.offset_y}).as_vec2();

    for (const Entity& bouncing_entity : bounceables) {
      if (bouncing_entity == entity) continue;

      const auto& bounceable_transform = component_manager.get_component<Transform>(bouncing_entity);
      const auto& box_collider = component_manager.get_component<BoxCollider>(bouncing_entity);

      if (!has_circle_aabb_overlap(circle, transform, box_collider, bounceable_transform)) {
        continue;
      }

      math::Vec2 normal = get_collision_normal(circle_center, box_collider, bounceable_transform);

      math::Vec2 reflected;

      const bool is_position_based = component_manager.has_component<PositionBasedBounce>(bouncing_entity);

      if (is_position_based) {
        const float box_center = bounceable_transform.position.x;
        const float hit_point = circle_center.x;
        const float distance = hit_point - box_center;

        const float normalized_offset = std::clamp(distance / (box_collider.width / 2.0f), -1.0F, 1.0F);
        const float max_angle = 60.0f * math::kDegToRad;

        float angle = normalized_offset * max_angle;
        reflected.x = std::sin(angle);
        reflected.y = -std::cos(angle); // always bounce upward

        reflected = reflected.normalized();
      }
      else {
        reflected = reflect(sb::math::Vec2{direction.x, direction.y}, normal).normalized();
      }

      // Clamp the reflected vector to a maximum of 0.2f. (Prevents orbiting.)
      if (std::abs(reflected.y) < 0.2f) {
        reflected.y = (reflected.y < 0 ? -1.0f : 1.0f) * 0.2f;
        reflected.x = std::copysign(std::sqrt(1.0f - reflected.y * reflected.y), reflected.x);
      }

      direction.x = reflected.x;
      direction.y = reflected.y;

      break;
    }
  }

  for (const auto& entity : clampables) {
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

bool sb::ecs::CollisionSystem::has_aabb_overlap(const BoxCollider& box_a,
    const Transform& transform_a, const BoxCollider& box_b, const Transform& transform_b) {

  const float a_left = transform_a.position.x + box_a.offset_x - (box_a.width / 2.0F);
  const float a_right = transform_a.position.x + box_a.offset_x + (box_a.width / 2.0F);
  const float a_top = transform_a.position.y + box_a.offset_y - (box_a.height / 2.0F);
  const float a_bottom = transform_a.position.y + box_a.offset_y + (box_a.height / 2.0F);

  const float b_left = transform_b.position.x + box_b.offset_x - (box_b.width / 2.0F);
  const float b_right = transform_b.position.x + box_b.offset_x + (box_b.width / 2.0F);
  const float b_top = transform_b.position.y + box_b.offset_y - (box_b.height / 2.0F);
  const float b_bottom = transform_b.position.y + box_b.offset_y + (box_b.height / 2.0F);

  return a_right >= b_left && a_left <= b_right && a_bottom >= b_top && a_top <= b_bottom;
}

bool sb::ecs::CollisionSystem::has_circle_aabb_overlap(const CircleCollider& circle,
    const Transform& circle_transform, const BoxCollider& box, const Transform& box_transform) {

  const float cx = circle_transform.position.x + circle.offset_x;
  const float cy = circle_transform.position.y + circle.offset_y;
  const float radius = circle.radius;

  const float box_x = box_transform.position.x + box.offset_x;
  const float box_y = box_transform.position.y + box.offset_y;
  const float box_width = box.width;
  const float box_height = box.height;

  const float half_width = box_width / 2.0F;
  const float half_height = box_height / 2.0F;

  const float closest_x = std::max(box_x - half_width, std::min(cx, box_x + half_width));
  const float closest_y = std::max(box_y - half_height, std::min(cy, box_y + half_height));

  const float dx = cx - closest_x;
  const float dy = cy - closest_y;

  return (dx * dx + dy * dy) < (radius * radius);
}

sb::math::Vec2 sb::ecs::CollisionSystem::get_collision_normal(const sb::math::Vec2& circle_center,
  const BoxCollider& box, const Transform& box_transform) {
  const float half_width = box.width / 2.0f;
  const float half_height = box.height / 2.0f;
  math::Vec2 box_center =
      (box_transform.position + sb::math::Vec2{box.offset_x, box.offset_y}).as_vec2();

  const float closest_x =
      std::max(box_center.x - half_width, std::min(circle_center.x, box_center.x + half_width));
  const float closest_y =
      std::max(box_center.y - half_height, std::min(circle_center.y, box_center.y + half_height));

  const math::Vec2 contact_point = {closest_x, closest_y};
  const math::Vec2 normal = circle_center - contact_point;

  return normal.normalized();
}