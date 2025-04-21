//
// Created by Jace Shultz (spaceofjace) on 4/21/2025.
//

#ifndef IENTITYMANAGER_H
#define IENTITYMANAGER_H

#include "Entity.h"

#include <unordered_set>
#include <vector>

class IEntityManager {
public:
  [[nodiscard]] virtual Entity CreateEntity() = 0;
  virtual bool DestroyEntity(Entity entity) = 0;
  [[nodiscard]] virtual bool IsAlive(Entity entity) const = 0;
  [[nodiscard]] virtual const std::unordered_set<Entity>&
    GetAllEntities() const = 0;
  virtual void ClearAll() = 0;
  virtual ~IEntityManager() = default;
};
#endif //IENTITYMANAGER_H
