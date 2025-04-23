//
// Created by Jace Shultz on 4/21/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <functional>

#include "ecs_config.h"

namespace sb::ecs {

struct Entity {
  EntityId id;
  Version version;

  static Entity null() { return {UINT32_MAX, UINT32_MAX}; }
  [[nodiscard]] bool is_null() const { return id == UINT32_MAX; }

  bool operator==(const Entity& other) const { return id == other.id && version == other.version; }
  bool operator!=(const Entity& other) const { return !(*this == other); }
};

} // namespace sb::ecs

template <>
struct std::hash<sb::ecs::Entity> {
  std::size_t operator()(const sb::ecs::Entity& e) const noexcept {
    const std::size_t idHash = std::hash<sb::ecs::EntityId>{}(e.id);
    const std::size_t versionHash = std::hash<sb::ecs::Version>{}(e.version);

    // combine hashes using XOR and bit shift similar to boost's hash_combine
    // https://www.boost.org/doc/libs/1_55_0/doc/html/hash/reference.html#boost.hash_combine
    return idHash ^ (versionHash << 1);
  }
};

#endif //ENTITY_H
