//
// Created by Jace Shultz on 7/23/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file GameData.h
 * @ingroup GameConfig
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
#include <string>

/**
 * @struct Vec2
 * @brief Simple 2D vector used for positional offsets and entity placement.
 *
 * Represents an x/y coordinate in screen space. Intended for use in positioning game entities
 * relative to the arena or to other entities. Will likely move into another file eventually
 */
struct Vec2 {
  float x = 0.0F;
  float y = 0.0F;
};

namespace sb::data {
/**
 * @struct LevelData
 * @brief Encapsulates all configuration data for a single level or arena setup.
 *
 * This structure defines the spatial layout of the play area (arena), the spawn parameters for the
 * paddle and ball, and spacing margins for walls or gameplay. Can be constructed directly or parsed
 * from a JSON config in the future.
 */
struct LevelData {
  int level_id = -1;                            /**< Unique identifier for the level */
  std::string level_name;                       /**< Optional level title or name */

  float arena_width = 640.0F;                   /**< Width of the gameplay arena in pixels */
  float arena_height = 480.0F;                  /**< Height of the gameplay arena in pixels */
  float wall_thickness = 16.0F;                 /**< Thickness of wall colliders around arena */

  // Paddle Data
  float paddle_width = 16.0F;                   /**< Width of the paddle */
  float paddle_height = 128.0F;                 /**< Height of the paddle */
  Vec2 paddle_offset = { 0.0F, 0.0F };    /**< Offset from arena bottom center */

  // Ball Data
  float ball_radius = 16.0F;                    /**< Radius of the ball */
  Vec2 ball_offset = { 0.0F, 0.0F };      /**< Offset relative to the paddle */

  // Arena Margins
  float bottom_margin = 16.0F;                  /**< Margin between bottom wall and arena edge */
  float top_margin = 16.0F;                     /**< Margin between top wall and arena edge */
  float left_margin = 16.0F;                    /**< Margin between left wall and arena edge */
  float right_margin = 16.0F;                   /**< Margin between right wall and arena edge */
};
} //namespace sb::data
#endif //GAMEDATA_H
