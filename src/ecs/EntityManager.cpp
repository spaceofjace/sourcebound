//
// Created by Jace Shultz (spaceofjace)on 4/21/2025.
//

#include "../../include/ecs/EntityManager.h"

#include "../../include/ecs/ecs_config.h"
#include "../../include/logger/sinks/ConsoleSink.h"

using namespace sb::ecs;

EntityManager::EntityManager(const bool enableReuse, const std::optional<uint32_t> maxReusePoolSize)
  : reuse_enabled_(enableReuse), max_reuse_pool_size_(maxReusePoolSize){

  if (enableReuse) {
    recycled_entities_.emplace();
  }
}

Entity EntityManager::create_entity() {
  std::optional<Entity> newEntity;
  if (entities_.size() == MAX_ENTITIES) {
    sb::log::Logger::error("[EntityManager] Max entities reached!");
    throw std::runtime_error("Max entities reached!");
  }

  if (reuse_enabled_ && !recycled_entities_->empty()) {
    newEntity = recycled_entities_->top();
    recycled_entities_->pop();
  } else {
    newEntity = Entity{ next_entity_id_, 0};
    next_entity_id_++;
  }

  entities_.insert(*newEntity);
  return *newEntity;
}

bool EntityManager::destroy_entity(const Entity entity) {
  const auto foundEntity = entities_.find(entity);
  if (foundEntity == entities_.end()) {
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
  return true;
}

bool EntityManager::is_alive(const Entity entity) const {
  return entities_.find(entity) != entities_.end();
}

const std::unordered_set<Entity> &EntityManager::get_all_entities() const {
  return entities_;
}

void EntityManager::clear_all() {
  entities_.clear();
  if (reuse_enabled_) {
    recycled_entities_.reset();
  }
}
