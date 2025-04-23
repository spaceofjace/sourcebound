//
// Created by Jace Shultz (spaceofjace) on 4/21/2025.
//

#ifndef IENTITYMANAGER_H
#define IENTITYMANAGER_H

#include "Entity.h"

#include <unordered_set>

namespace sb::ecs {

class IEntityManager {
public:
  virtual ~IEntityManager() = default;

  IEntityManager() = default;
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
};

} // namespace sb::ecs
#endif //IENTITYMANAGER_H
