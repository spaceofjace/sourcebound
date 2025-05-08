//
// Created by Jace Shultz on 5/8/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef MOCKENTITYMANAGER_H
#define MOCKENTITYMANAGER_H

#include "../../include/ecs/IEntityManager.h"
#include "../../include/ecs/Entity.h"
#include "../../include/ecs/ecs_config.h"
#include <unordered_map>
#include <unordered_set>

using namespace sb::ecs;

struct MockEntityManager : IEntityManager {
  Entity next_entity{42, 0};
  std::unordered_map<EntityId, Signature> signatures;
  std::unordered_set<Entity> entities;

  Entity create_entity() override {
    entities.insert(next_entity);
    set_signature(next_entity.id, Signature{});
    return next_entity;
  }

  bool destroy_entity(const Entity entity) override {
    if (!is_alive(entity)) {
      return false;
    }

    entities.erase(next_entity);
    return true;
  }

  [[nodiscard]] bool is_alive(const Entity entity) const override {
    return entities.find(entity) != entities.end();
  }

  [[nodiscard]] Signature get_signature(const EntityId id) const override {
    auto it = signatures.find(id);
    if (it != signatures.end()) {
      return it->second;
    }
    return Signature{}; //safe default for mocking
  }

  void set_signature(const EntityId id, const Signature sig) override {
    signatures[id] = sig;
  }
  [[nodiscard]] const std::unordered_set<Entity>& get_all_entities() const override {
    return entities;
  }
  void clear_all() override {
    entities.clear();
  }

  [[nodiscard]] bool try_get_signature(EntityId entityId, Signature& signature) const override {
    return false;
  }
};
#endif //MOCKENTITYMANAGER_H
