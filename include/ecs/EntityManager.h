//
// Created by Jace Shultz on 4/21/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file EntityManager.h
 * @ingroup ECS
 * @brief Concrete implementation of IEntityManager (includes option for reuse)
 */

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include "IEntityManager.h"
#include <optional>
#include <stack>
#include <unordered_set>

namespace sb::ecs {

/**
 * @class EntityManager
 * @ingroup ECS
 * @brief Concrete manager of entity lifetime and ensures uniqueness of alive entities.
 *
 * A concrete implementation of IEntityManager that uses versioning to support potential entity
 * reuse (with a specific pool size), if desired.
 */

class EntityManager final : public IEntityManager {
public:
  EntityManager(const EntityManager&) = delete;
  EntityManager& operator=(const EntityManager&) = delete;
  EntityManager(EntityManager&&) = delete;
  EntityManager& operator=(EntityManager&&) = delete;

  explicit EntityManager(bool enableReuse = false, std::optional<uint32_t> maxReusePoolSize = std::nullopt);
  Entity create_entity() override;
  bool destroy_entity(Entity entity) override;
  [[nodiscard]] bool is_alive(Entity entity) const override;
  [[nodiscard]] const std::unordered_set<Entity> & get_all_entities() const override;
  [[nodiscard]] std::vector<Entity> get_entities_with_signature(
    const Signature& target_signature) const override;
  void set_signature(EntityId entityId, Signature signature) override;
  [[nodiscard]] Signature get_signature(EntityId entityId) const override;
  [[nodiscard]] bool try_get_signature(EntityId entityId, Signature& signature) const override;
  void clear_all() override;
  ~EntityManager() override = default;
private:
  std::unordered_set<Entity> entities_;
  std::unordered_map<uint32_t, Signature> signatures_;
  uint32_t next_entity_id_ = 0;

  bool reuse_enabled_ = false;
  //Values used if reuse is enabled
  std::optional<uint32_t> max_reuse_pool_size_ = std::nullopt;
  std::optional<std::stack<Entity>> recycled_entities_ = std::nullopt;
};

}  // namespace sb::ecs
#endif //ENTITYMANAGER_H
