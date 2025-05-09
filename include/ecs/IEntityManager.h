//
// Created by Jace Shultz on 4/21/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef IENTITYMANAGER_H
#define IENTITYMANAGER_H

#include <unordered_set>

#include "Entity.h"
#include "ecs_config.h"

namespace sb::ecs {

class IEntityManager {
public:
  IEntityManager() = default;
  virtual ~IEntityManager() = default;

  IEntityManager(const IEntityManager&) = delete;
  IEntityManager& operator=(const IEntityManager&) = delete;
  IEntityManager(IEntityManager&&) = delete;
  IEntityManager& operator=(IEntityManager&&) = delete;

  [[nodiscard]] virtual Entity create_entity() = 0;
  virtual bool destroy_entity(Entity entity) = 0;
  [[nodiscard]] virtual bool is_alive(Entity entity) const = 0;
  [[nodiscard]] virtual const std::unordered_set<Entity>&
    get_all_entities() const = 0;
  virtual void clear_all() = 0;
  virtual void set_signature(EntityId entityId, Signature signature) = 0;
  [[nodiscard]] virtual Signature get_signature(EntityId entityId) const = 0;
  [[nodiscard]] virtual bool try_get_signature(EntityId entityId, Signature& signature) const = 0;
};

} // namespace sb::ecs
#endif //IENTITYMANAGER_H
