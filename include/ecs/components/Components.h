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
#include "../../rendering/Color.h"
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
 * @struct CircleCollider
 * @ingroup ECS
 * @brief Defines a circular collision boundary for an entity.
 *
 * Used by the physics or collision system to determine interactions with other colliders.
 * Offsets are relative to the entity's origin (typically the center or top-left corner).
 */
struct CircleCollider {
  float radius = 0.0F;
  float offset_x = 0.0F;
  float offset_y = 0.0F;
};

/**
 * @struct BoxCollider
 * @ingroup ECS
 * @brief Defines an axis-aligned rectangular collision boundary for an entity.
 *
 * Used by the physics or collision system for bounding box intersection checks.
 * Offsets are relative to the entity's origin (typically the top-left corner unless otherwise specified).
 */
struct BoxCollider {
  float width = 0.0F;
  float height = 0.0F;
  float offset_x = 0.0F;
  float offset_y = 0.0F;
};

//Rendering components
/**
 * @enum SimpleShapeType
 * @ingroup Rendering
 * @brief Enumerates supported primitive shape types for rendering.
 *
 * Used in conjunction with RenderableSimpleShape to define basic visual geometry.
 */
enum class SimpleShapeType {
  Invalid = -1,   ///< Unset or unsupported shape.
  Rectangle,      ///< A standard axis-aligned rectangle.
  Circle,         ///< A simple filled or outlined circle.
};

/**
 * @struct RenderableSimpleShape
 * @ingroup ECS
 * @brief Describes a basic shape to render for an entity.
 *
 * Used by the render system to draw simple geometric shapes. Shape type is defined by
 * SimpleShapeType. This component is purely visual and separate from physics.
 */
struct RenderableSimpleShape {
  sb::rendering::Color color;
  SimpleShapeType type;
  bool filled{};

  explicit RenderableSimpleShape(const sb::rendering::Color& color, const SimpleShapeType type,
    const bool filled)
      : color(color), type(type), filled(filled) {}
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
