//
// Created by Jace Shultz on 8/1/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
* @file ArenaDimensions.h
* @ingroup Data
* @brief Provides calculations for arena and window size based on level configuration.
*
* Computes the interior playable space (arena) and full window dimensions,
* including outer margins and wall thickness, using values from LevelData.
*
* @author Jace Shultz
* @date 8/1/2025
*/
#ifndef ARENADIMENSIONS_H
#define ARENADIMENSIONS_H

#include "../math/Vec2.h"
#include "LevelData.h"

namespace sb::data {

using math::Vec2;
using data::LevelData;

/**
* @struct ArenaDimensions
* @brief Represents the calculated size of the playable arena and the overall window.
*
* The arena size defines the internal gameplay space (excluding margins and walls).
* The window size includes all decorative or UI framing around the arena.
*/
struct ArenaDimensions {
  Vec2 arena_size;    /**< Width and height of the playable arena. */
  Vec2 window_size;   /**< Total size of the window, including margins and wall space. */
};

/**
* @brief Calculates arena and window dimensions from the level configuration.
*
* Includes space for brick grid layout, wall thickness, inner and outer margins,
* and vertical buffer for paddle and ball clearance.
*
* @param level LevelData containing brick layout and spacing parameters.
* @return ArenaDimensions with computed arena and window sizes.
*/
inline ArenaDimensions calculate_arena_dimensions(const LevelData& level) {
  const auto& layout = level.brick_layout;
  const auto& num_rows = level.bricks.size();
  const auto& num_cols = level.bricks[0].size();

  // Paddle Gameplay Space at the lower part
  const float paddle_space = level.paddle_offset.y + level.paddle_height;
  const float ball_space = level.ball_offset.y + (level.ball_radius * 2);
  const float buffer = paddle_space + ball_space + level.wall_thickness + 4.0F; //extra padding

  const float arena_width = (static_cast<float>(num_cols) * layout.brick_size.x) +
    ((static_cast<float>(num_cols) - 1) * layout.brick_margin.x) + level.inner_left_margin
      + level.inner_right_margin;

  const float arena_height = buffer + (static_cast<float>(num_rows) * layout.brick_size.y) +
    ((static_cast<float>(num_rows) - 1) * layout.brick_margin.y) + level.inner_top_margin
      + level.inner_bottom_margin;

  const float window_width = arena_width + level.outer_left_margin + level.outer_right_margin
    + (2.0F * level.wall_thickness);
  const float window_height = arena_height + level.outer_top_margin + level.outer_bottom_margin
    + (2.0F * level.wall_thickness);

  return ArenaDimensions{
    Vec2{arena_width, arena_height},
    Vec2{window_width, window_height}
  };
}
} //namespace sb::data
#endif //ARENADIMENSIONS_H
