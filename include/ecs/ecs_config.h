//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef ECS_CONFIG_H
#define ECS_CONFIG_H

#include <bitset>
#include <cstddef>

namespace sb::ecs {

constexpr std::size_t MAX_ENTITIES = 5000;
constexpr std::size_t MAX_COMPONENTS = 64;

using Signature = std::bitset<MAX_COMPONENTS>;
using EntityId = std::uint32_t;
using Version = std::uint32_t;
using ComponentType = std::size_t;

} // namespace sb::ecs
#endif //ECS_CONFIG_H
