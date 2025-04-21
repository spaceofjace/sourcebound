//
// Created by Jace Shultz (spaceofjace) on 4/21/2025.
//

#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <functional>

struct Entity {
  uint32_t id;
  uint32_t version;

  bool operator==(const Entity& other) const { return id == other.id && version == other.version; }
  bool operator!=(const Entity& other) const { return !(*this == other); }
};

template <> struct std::hash<Entity> {
  std::size_t operator()(const Entity &e) const noexcept {
    const std::size_t idHash = std::hash<uint32_t>{}(e.id);
    const std::size_t versionHash = std::hash<uint32_t>{}(e.version);

    // combine hashes using XOR and bit shift similar to boost's hash_combine
    // https://www.boost.org/doc/libs/1_55_0/doc/html/hash/reference.html#boost.hash_combine
    return idHash ^ (versionHash << 1);
  }
};

#endif //ENTITY_H
