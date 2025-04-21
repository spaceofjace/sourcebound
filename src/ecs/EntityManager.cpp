//
// Created by Jace Shultz (spaceofjace)on 4/21/2025.
//

#include "../../include/ecs/EntityManager.h"

EntityManager::EntityManager(const bool enableReuse, const std::optional<uint32_t> maxReusePoolSize)
  : enableReuse(enableReuse), maxReusePoolSize(maxReusePoolSize){

  if (enableReuse) {
    recycledEntities.emplace();
  }
}

Entity EntityManager::CreateEntity() {
  std::optional<Entity> newEntity;
  if (enableReuse && !recycledEntities->empty()) {
    newEntity = recycledEntities->top();
    recycledEntities->pop();
  } else {
    newEntity = Entity{ nextEntityId, 0};
    nextEntityId++;
  }

  entities.insert(*newEntity);
  return *newEntity;
}

bool EntityManager::DestroyEntity(const Entity entity) {
  const auto foundEntity = entities.find(entity);
  if (foundEntity == entities.end()) { return false; }

  if (enableReuse) {
    if (!maxReusePoolSize || recycledEntities->size() < *maxReusePoolSize) {
      Entity updatedEntity = *foundEntity;
      updatedEntity.version++;
      recycledEntities->push(updatedEntity);
    }
  }

  entities.erase(foundEntity);
  return true;
}

bool EntityManager::IsAlive(const Entity entity) {
  return entities.find(entity) != entities.end();
}

const std::unordered_set<Entity> &EntityManager::GetAllEntities() const {
  return entities;
}

void EntityManager::ClearAll() {
  entities.clear();
  if (enableReuse) {
    recycledEntities.reset();
  }
}
