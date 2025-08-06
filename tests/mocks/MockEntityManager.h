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
#include <gmock/gmock.h>

using namespace sb::ecs;

struct MockEntityManager : IEntityManager {
  Entity next_entity{41, 0};
  std::unordered_map<EntityId, Signature> signatures;
  std::unordered_set<Entity> entities;

  Entity create_entity() override {
    next_entity.id++;
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

  [[nodiscard]] std::vector<Entity> get_entities_with_signature(
    const Signature& target_signature) const override {
    std::vector<Entity> result;
    for (const Entity& entity : entities) {
      auto iterator = signatures.find(entity.id);
      if (iterator != signatures.end() && (iterator->second & target_signature) == target_signature) {
        result.push_back(entity);
      }
    }
    return result;
  }
};

//GMOCK VERSION - will update older unit tests in a different CL
class MockEntityManager_GMock final : public sb::ecs::IEntityManager {
public:
  MOCK_METHOD(sb::ecs::Entity, create_entity, (), (override));
  MOCK_METHOD(bool, destroy_entity, (sb::ecs::Entity), (override));
  MOCK_METHOD(bool, is_alive, (sb::ecs::Entity), (const, override));
  MOCK_METHOD(sb::ecs::Signature, get_signature, (sb::ecs::EntityId), (const, override));
  MOCK_METHOD(void, set_signature, (sb::ecs::EntityId, sb::ecs::Signature), (override));
  MOCK_METHOD(const std::unordered_set<sb::ecs::Entity>&, get_all_entities, (), (const, override));
  MOCK_METHOD(void, clear_all, (), (override));
  MOCK_METHOD(bool, try_get_signature, (sb::ecs::EntityId, sb::ecs::Signature&), (const, override));
  MOCK_METHOD(std::vector<sb::ecs::Entity>, get_entities_with_signature,
              (const sb::ecs::Signature&), (const, override));
};

#endif //MOCKENTITYMANAGER_H
