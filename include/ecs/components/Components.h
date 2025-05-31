//
// Created by Jace Shultz on 4/21/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file Components.h
 * @ingroup ECS
 * @brief Defines core gameplay component structures for ECS entities.
 *
 * This file contains a set of commonly used component types, including physics, rendering, and
 * gameplay-related data. Intended for rapid prototyping and small-scale projects, all components
 * are kept in one place for simplicity.
 *
 * As the component set expands, it may be beneficial to refactor into module-specific files.
 */

#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <string>

// Physics components

/**
 * @struct Position
 * @ingroup ECS
 * @brief Represents 2D position in coordinate system.
 */
struct Position { float x = 0.f, y = 0.f; };

/**
 * @struct Velocity
 * @ingroup ECS
 * @brief Provides velocity vectors in two dimensions.
 */
struct Velocity { float x = 0.f, y = 0.f; };

/**
 * @struct Size
 * @ingroup ECS
 * @brief Physical width and height scalars.
 */
struct Size     { float width = 1.f, height = 1.f; };

/**
 * @struct Rotation
 * @ingroup ECS
 * @brief Provides rotational angle.
 */
struct Rotation { float angle = 0.f; };

/**
 * @struct Transform
 * @ingroup ECS
 * @brief Combines position, velocity, size, and rotation into a unified spatial representation.
 */
struct Transform {
  Position position;
  Velocity velocity;
  Size size;
  Rotation rotation;

  explicit Transform(const Position p = {}, const Velocity v = {},
    const Size s = {}, const Rotation r = {})
    : position(p), velocity(v), size(s), rotation(r) {}
};

/**
 * @struct Collider
 * @ingroup ECS
 * @brief Data for if object should be considered a collider.
 *
 * Possible this should include separate collider size in the future, but assuming simplest case
 */
struct Collider { bool isTrigger = false; };

//Graphics components
/**
 * @struct Color
 * @ingroup ECS
 * @brief Color data for rendering (including alpha).
 */
struct Color { float r, g, b, a; };

/**
 * @struct Sprite
 * @ingroup ECS
 * @brief Sprite data for rendering (including color).
 */
struct Sprite {
  std::string texturePath;
  Color color;
};

//Gameplay components
//These are assumed to be specific to the game being implemented, and should probably be separated
//into their own file eventually.

/**
 * @struct HitsRequired
 * @ingroup ECS
 * @brief Number of hits required to destroy a block.
 */
struct HitsRequired { int hits; };

/**
 * @struct Ball
 * @ingroup ECS
 * @brief A "tag" component to indicate a ball in the game
 */
struct Ball { };

/**
 * @struct Paddle
 * @ingroup ECS
 * @brief A "tag" component to indicate a paddle in the game
 */
struct Paddle { };

#endif //COMPONENTS_H
