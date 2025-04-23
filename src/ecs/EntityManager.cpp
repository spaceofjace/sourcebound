//
// Created by Jace Shultz on 4/21/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/ecs/EntityManager.h"
#include "../../include/ecs/ecs_config.h"
#include "../../include/logger/sinks/ConsoleSink.h"

namespace sb::ecs{

EntityManager::EntityManager(const bool enableReuse, const std::optional<uint32_t> maxReusePoolSize)
  : reuse_enabled_(enableReuse), max_reuse_pool_size_(maxReusePoolSize){

  if (enableReuse) {
    recycled_entities_.emplace();
  }
}

Entity EntityManager::create_entity() {
  std::optional<Entity> newEntity;
  if (entities_.size() == MAX_ENTITIES) {
    log::Logger::error("[EntityManager] Max entities reached!");
    return Entity::null();
  }

  if (reuse_enabled_ && !recycled_entities_->empty()) {
    newEntity = recycled_entities_->top();
    recycled_entities_->pop();
  } else {
    newEntity = Entity{ next_entity_id_, 0};
    next_entity_id_++;
  }

  entities_.insert(*newEntity);
  signatures_[newEntity->id] = Signature{};
  return *newEntity;
}

bool EntityManager::destroy_entity(const Entity entity) {
  const auto foundEntity = entities_.find(entity);
  if (foundEntity == entities_.end()) {
    log::Logger::warn("[EntityManager] Attempted to destroy non-existent entity.");
    return false;
  }

  if (reuse_enabled_) {
    if (!max_reuse_pool_size_ || recycled_entities_->size() < *max_reuse_pool_size_) {
      Entity updatedEntity = *foundEntity;
      updatedEntity.version++;
      recycled_entities_->push(updatedEntity);
    }
  }

  entities_.erase(foundEntity);

  if (const auto signature = signatures_.find(entity.id); signature != signatures_.end()) {
    signatures_.erase(signature);
  } else {
    //A corrupt state that should not occur, but checked here just in case
    log::Logger::warn("[EntityManager] Signature not set before destroy!");
  }

  return true;
}

bool EntityManager::is_alive(const Entity entity) const {
  return entities_.find(entity) != entities_.end();
}

const std::unordered_set<Entity>& EntityManager::get_all_entities() const {
  return entities_;
}

void EntityManager::set_signature(const EntityId entityId, const Signature signature) {

  if (signature == get_signature(entityId)) {
    log::Logger::warn("[EntityManager] Signature already set!");
    return;
  }

  signatures_[entityId] = signature;
}

Signature EntityManager::get_signature(const EntityId entityId) const {
  Signature signature;
  return try_get_signature(entityId, signature) ? signature : Signature{};
}

bool EntityManager::try_get_signature(const EntityId entityId, Signature& signature) const {
  if (signatures_.find(entityId) == signatures_.end()) {
    log::Logger::warn("[EntityManager] Signature not set!");
    return false;
  }
  signature = signatures_.at(entityId);
  return true;
}

void EntityManager::clear_all() {
  entities_.clear();
  if (reuse_enabled_) {
    recycled_entities_.reset();
  }
}

}  // namespace sb::ecs
