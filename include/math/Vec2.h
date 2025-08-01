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

#include <cmath>

//Forward declaration
namespace sb::ecs {
  struct Position;
}

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

  /**
  * @brief Returns the magnitude (length) of the vector.
  */
  float magnitude() const {
    return std::sqrt(x * x + y * y);
  }

  /**
  * @brief Returns a normalized (unit length) version of the vector.
  *        If the vector is zero-length, returns (0, 0).
  */
  Vec2 normalized() const {
    const float mag = magnitude();
    if (mag == 0.0F) return Vec2{0.0F, 0.0F};
    return Vec2{x / mag, y / mag};
  }

  Vec2 operator+(const Vec2& rhs) const {
    return Vec2{x + rhs.x, y + rhs.y};
  }

  Vec2 operator-(const Vec2& rhs) const {
    return Vec2{x - rhs.x, y - rhs.y};
  }

  Vec2 operator*(const float rhs) const {
    return Vec2{x * rhs, y * rhs};
  }
};

inline Vec2 operator*(const float lhs, const Vec2& rhs) {
  return Vec2{lhs * rhs.x, lhs * rhs.y};
}

inline float dot(const Vec2& a, const Vec2& b) {
  return a.x * b.x + a.y * b.y;
}

inline Vec2 reflect(const Vec2& incoming, const Vec2& normal) {
  return incoming - 2.0F * dot(incoming, normal) * normal;
}
} // namespace sb::math

#endif //VEC2_H
