//
// Created by Jace Shultz on 7/23/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file LevelData.h
 * @ingroup Data
 * @brief Contains data structures for level configuration and arena layout.
 *
 * This header defines serializable and configurable gameplay data, including arena dimensions,
 * paddle/ball parameters, and margin spacing. Intended to be populated via hardcoded values or
 * JSON deserialization in future stages.
 *
 * @author Jace Shultz
 * @date 7/23/2025
 */
#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <optional>
#include <string>

#include "../math/Vec2.h"
#include "BrickConfig.h"
#include "BrickLayoutConfig.h"

namespace sb::data {
/**
 * @struct LevelData
 * @brief Encapsulates all configuration data for a single level or arena setup.
 *
 * This structure defines the spatial layout of the play area (arena), the spawn parameters for the
 * paddle and ball, and spacing margins for walls or gameplay. Can be constructed directly or parsed
 * from a JSON config in the future.
 */

using math::Vec2;

struct LevelData {
  int level_id = -1;                            /**< Unique identifier for the level */
  std::string level_name;                       /**< Optional level title or name */

  float wall_thickness = 8.0F;                 /**< Thickness of wall colliders around arena */

  // Paddle Data
  float paddle_width = 100.0F;                  /**< Width of the paddle */
  float paddle_height = 16.0F;                  /**< Height of the paddle */
  Vec2 paddle_offset = { 0.0F, -6.0F };   /**< Offset from arena bottom center */
  float paddle_speed = 200.0F;                  /**< Paddle speed in pixels per second */

  // Ball Data
  float ball_radius = 12.0F;                    /**< Radius of the ball */
  Vec2 ball_offset = { 0.0F, 0.0F };      /**< Offset relative to the paddle */
  float ball_speed = 200.0F;                    /**< Ball speed in pixels per second */

  // Margins
  float outer_bottom_margin = 16.0F;            /**< Margin outside bottom wall and arena edge */
  float outer_top_margin = 16.0F;               /**< Margin outside top wall and arena edge */
  float outer_left_margin = 16.0F;              /**< Margin outside left wall and arena edge */
  float outer_right_margin = 16.0F;             /**< Margin outside right wall and arena edge */

  float inner_bottom_margin = 16.0F;            /**< Margin inside bottom wall and brick grid */
  float inner_top_margin = 16.0F;               /**< Margin inside top wall and brick grid */
  float inner_left_margin = 16.0F;              /**< Margin inside left wall and brick grid */
  float inner_right_margin = 16.0F;             /**< Margin inside right wall and brick grid */

  // Brick Data
  BrickLayoutConfig brick_layout;               /**< Layout configuration for bricks */
  std::vector<std::vector<std::optional<BrickConfig>>> bricks; /**< Grid of bricks */
};
} //namespace sb::data
#endif //GAMEDATA_H
