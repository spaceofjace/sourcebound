//
// Created by Jace Shultz (spaceofjace) on 4/21/2025.
//

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include "IEntityManager.h"
#include <optional>
#include <stack>
#include <unordered_set>

class EntityManager final : public IEntityManager {
public:
  explicit EntityManager(bool enableReuse = false, std::optional<uint32_t> maxReusePoolSize = std::nullopt);
  Entity CreateEntity() override;
  bool DestroyEntity(Entity entity) override;
  bool IsAlive(Entity entity) override;
  [[nodiscard]] const std::unordered_set<Entity> &
  GetAllEntities() const override;
  void ClearAll() override;
  ~EntityManager() override = default;
private:
  std::unordered_set<Entity> entities;
  uint32_t nextEntityId = 0;

  bool enableReuse = false;
  //Values used if reuse is enabled
  std::optional<uint32_t> maxReusePoolSize = std::nullopt;
  std::optional<std::stack<Entity>> recycledEntities = std::nullopt;
};

#endif //ENTITYMANAGER_H
