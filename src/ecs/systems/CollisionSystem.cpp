//
// Created by Jace Shultz on 7/30/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <algorithm>
#include "../../../include/ecs/systems/CollisionSystem.h"
#include "../../../include/math/MathConsts.h"
#include "../../../include/gamestate/GameWorld.h"

void sb::ecs::CollisionSystem::update(float /*delta_time*/, gamestate::GameWorld& game_world) {
  std::vector<Entity> clampables;
  std::vector<Entity> clamp_bounds;

  std::vector<Entity> bounceables;

  std::vector<Entity> triggerables;

  std::vector snapshot(entities.begin(), entities.end());
  for (const Entity& entity : snapshot) {
    if (!game_world.has_component<BoxCollider>(entity)
      && game_world.has_component<CircleCollider>(entity)) { continue; }

    const auto& box = game_world.get_component<BoxCollider>(entity);

    if ((box.behavior & CollisionBehavior::Bounce) == CollisionBehavior::Bounce) {
      bounceables.push_back(entity);
    }

    if ((box.behavior & CollisionBehavior::Clamp) == CollisionBehavior::Clamp) {
      clamp_bounds.push_back(entity);
    }
    else if ((box.behavior & CollisionBehavior::Trigger) == CollisionBehavior::Trigger) {
      triggerables.push_back(entity);
    }

    const bool is_clampable =
      game_world.has_component<Direction>(entity) &&
      game_world.has_component<Paddle>(entity);

    if (is_clampable) {
      clampables.push_back(entity);
    }
  }

  for (const Entity& entity : snapshot) {
    if (!game_world.has_component<Transform>(entity) ||
        !game_world.has_component<Direction>(entity) ||
        !game_world.has_component<CircleCollider>(entity)) { continue; }

    auto& transform = game_world.get_component<Transform>(entity);
    auto& direction = game_world.get_component<Direction>(entity);
    const auto& circle = game_world.get_component<CircleCollider>(entity);

    const math::Vec2 circle_center =
        (transform.position + sb::math::Vec2{circle.offset_x, circle.offset_y}).as_vec2();

    for (const Entity& bouncing_entity : bounceables) {
      if (bouncing_entity == entity) continue;

      const auto& bounceable_transform = game_world.get_component<Transform>(bouncing_entity);
      const auto& box_collider = game_world.get_component<BoxCollider>(bouncing_entity);

      if (!has_circle_aabb_overlap(circle, transform, box_collider, bounceable_transform)) {
        continue;
      }

      math::Vec2 normal = get_collision_normal(circle_center, box_collider, bounceable_transform);

      math::Vec2 reflected;

      const bool is_position_based = game_world.has_component<PositionBasedBounce>(bouncing_entity);

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
    auto& transform = game_world.get_component<Transform>(entity);
    const auto& collider = game_world.get_component<BoxCollider>(entity);
    const auto& direction = game_world.get_component<Direction>(entity);

    for (const auto& boundary : clamp_bounds) {
      if (boundary == entity) { continue; }

      const auto& boundary_transform = game_world.get_component<Transform>(boundary);
      const auto& boundary_collider = game_world.get_component<BoxCollider>(boundary);

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

  for (const auto& entity : triggerables) {
    if (!game_world.has_component<BoxCollider>(entity) ||
      !game_world.has_component<Transform>(entity)) {
      continue;
      }

    const auto& trigger_transform = game_world.get_component<Transform>(entity);
    const auto& trigger_collider = game_world.get_component<BoxCollider>(entity);

    for (const auto& other : entities) {
      if (other == entity) continue;
      if (!game_world.has_component<Transform>(other)) continue;

      if (game_world.has_component<BoxCollider>(other)) {
        const auto& other_transform = game_world.get_component<Transform>(other);
        const auto& other_collider = game_world.get_component<BoxCollider>(other);

        if (has_aabb_overlap(trigger_collider, trigger_transform, other_collider, other_transform)) {
          game_world.add_component(entity, WasTriggered{other});
          break;
        }
      }
      else if (game_world.has_component<CircleCollider>(other)) {
        const auto& other_transform = game_world.get_component<Transform>(other);
        const auto& circle = game_world.get_component<CircleCollider>(other);

        if (has_circle_aabb_overlap(circle, other_transform, trigger_collider, trigger_transform)) {
          game_world.add_component(entity, WasTriggered{other});
          break;
        }
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

  auto collision_detected = (a_right > b_left + kEpsilon) && (a_left < b_right - kEpsilon) &&
       (a_bottom > b_top + kEpsilon) && (a_top < b_bottom - kEpsilon);

  if (collision_detected) {
    log::Logger::debug("[CollisionSystem]: AABB collision detected.");
    log::Logger::debug("Transform A: pos " + std::to_string(transform_a.position.x) + ", " + std::to_string(transform_a.position.y));
    log::Logger::debug("Transform B: pos " + std::to_string(transform_b.position.x) + ", " + std::to_string(transform_b.position.y));
    log::Logger::debug("AABB A: pos " + std::to_string(a_left) + ", " + std::to_string(a_top) + ", " + std::to_string(a_right) + ", " + std::to_string(a_bottom));
    log::Logger::debug("AABB B: pos " + std::to_string(b_left) + ", " + std::to_string(b_top) + ", " + std::to_string(b_right) + ", " + std::to_string(b_bottom));

    log::Logger::debug("BoxCollider A: width " + std::to_string(box_a.width) + ", height " + std::to_string(box_a.height));
    log::Logger::debug("BoxCollider B: width " + std::to_string(box_b.width) + ", height " + std::to_string(box_b.height));
  }

  return collision_detected;
}

bool sb::ecs::CollisionSystem::has_circle_aabb_overlap(const CircleCollider& circle,
    const Transform& circle_transform, const BoxCollider& box, const Transform& box_transform) {

  const float cx = circle_transform.position.x + circle.offset_x;
  const float cy = circle_transform.position.y + circle.offset_y;
  const float radius = circle.radius;

  const float box_center_x = box_transform.position.x + box.offset_x;
  const float box_center_y = box_transform.position.y + box.offset_y;
  const float half_width = box.width / 2.0F;
  const float half_height = box.height / 2.0F;

  const float box_left = box_center_x - half_width;
  const float box_right = box_center_x + half_width;
  const float box_top = box_center_y - half_height;
  const float box_bottom = box_center_y + half_height;

  const float closest_x = std::max(box_left, std::min(cx, box_right));
  const float closest_y = std::max(box_top, std::min(cy, box_bottom));

  const float dx = cx - closest_x;
  const float dy = cy - closest_y;

  const bool collision_detected = (dx * dx + dy * dy) < ((radius - kEpsilon) * (radius - kEpsilon));

  if (collision_detected) {
    log::Logger::debug("[CollisionSystem]: Circle-AABB collision detected.");
    log::Logger::debug("Circle Center: (" + std::to_string(cx) + ", " + std::to_string(cy) + ")");
    log::Logger::debug("Radius: " + std::to_string(radius));
    log::Logger::debug("Circle Offset: (" + std::to_string(circle.offset_x) + ", " + std::to_string(circle.offset_y) + ")");

    log::Logger::debug("Box Center: (" + std::to_string(box_center_x) + ", " + std::to_string(box_center_y) + ")");
    log::Logger::debug("Box Size: width " + std::to_string(box.width) + ", height " + std::to_string(box.height));
    log::Logger::debug("Box Bounds: left " + std::to_string(box_left) + ", right " + std::to_string(box_right) +
                       ", top " + std::to_string(box_top) + ", bottom " + std::to_string(box_bottom));

    log::Logger::debug("Closest Point on Box to Circle: (" + std::to_string(closest_x) + ", " + std::to_string(closest_y) + ")");
    log::Logger::debug("dx: " + std::to_string(dx) + ", dy: " + std::to_string(dy) +
                       ", dist^2: " + std::to_string(dx * dx + dy * dy));
  }

  return collision_detected;
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