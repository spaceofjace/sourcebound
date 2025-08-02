//
// Created by Jace Shultz on 8/1/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file BrickConfig.h
 * @ingroup Data
 * @brief Defines optional ECS component configuration for a single brick entity.
 * 
 * @author Jace Shultz
 * @date 8/1/2025
 */
#ifndef BRICKCONFIG_H
#define BRICKCONFIG_H
#include <optional>
#include "../ecs/components/Components.h"

namespace sb::data {

using ecs::HitsRequired;
using ecs::RenderableSimpleShape;
using ecs::BoxCollider;

/**
* @struct BrickConfig
* @brief Represents the set of optional ECS components used to construct a brick entity.
*
* A brick may include hitpoint data, visual rendering info, and a physics collider.
* Each field is optional to support data-driven, flexible level construction.
*/
struct BrickConfig {
  std::optional<HitsRequired> hits;             /**< Optional number of hits required to destroy the brick. */
  std::optional<RenderableSimpleShape> visual;  /**< Optional visual appearance of the brick. */
  std::optional<BoxCollider> collider;          /**< Optional physics collider used for interaction or bounce. */
};

}
#endif //BRICKCONFIG_H
