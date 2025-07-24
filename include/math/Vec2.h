//
// Created by Jace Shultz on 7/24/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file Vec2.h
 * @ingroup math
 * @brief Simple representation of a 2D Vector
 * 
 * @author Jace Shultz
 * @date 7/24/2025
 */
#ifndef VEC2_H
#define VEC2_H
namespace sb::math {
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

  bool operator==(const Vec2& rhs) const {
    return x == rhs.x && y == rhs.y;
  }
};
} // namespace sb::math

#endif //VEC2_H
