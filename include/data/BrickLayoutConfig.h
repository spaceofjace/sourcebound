//
// Created by Jace Shultz on 8/1/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file BrickLayoutConfig.h
 * @ingroup Data
 * @brief Describes global layout properties for brick grids in a level.
 * 
 * @author Jace Shultz
 * @date 8/1/2025
 */
#ifndef BRICKLAYOUTCONFIG_H
#define BRICKLAYOUTCONFIG_H

#include "../math/Vec2.h"

namespace sb::data {

using math::Vec2;

/**
* @struct BrickLayoutConfig
* @brief Defines dimensions and spacing for bricks in the level.
*
* BrickLayoutConfig applies to the full brick grid and is used when calculating
* arena size and positioning individual bricks.
*/
struct BrickLayoutConfig {
  Vec2 brick_size{64.0F, 32.0F};     /**< Size of each brick (width, height) in pixels. */
  Vec2 brick_margin{4.0F, 4.0F};     /**< Spacing between adjacent bricks (x/y direction). */
};
}  // namespace sb::data
#endif //BRICKLAYOUTCONFIG_H
