//
// Created by Jace Shultz on 5/8/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef GAMEWORLD_H
#define GAMEWORLD_H
#include <memory>

#include "../ecs/Entity.h"
#include "../ecs/IEntityManager.h"
#include "../ecs/IComponentManager.h"
#include "IGameWorld.h"

namespace sb::ecs {

// GameWorld is acting as a facade for common ECS operations; this serves two purposes:
//  1. It provides a single point of entry and allows for certain operations to be correctly managed
//      (e.g., component signature management on entities)
//  2. It *theoretically* allows for this facade to be swapped with a more robust library,
//      such as flecs or something similar.

template <typename EntityMgr, typename ComponentMgr, typename SystemMgr>
class GameWorld final : public IGameWorld {
 public:
  GameWorld(std::shared_ptr<EntityMgr> entity_manager,
            std::shared_ptr<ComponentMgr> component_manager,
            std::shared_ptr<SystemMgr> system_manager)
      : entity_manager_(std::move(entity_manager)),
        component_manager_(std::move(component_manager)),
        system_manager_(std::move(system_manager)) {}

  Entity create_entity() override {
    return entity_manager_->create_entity();
  }

  void destroy_entity(Entity entity) override {
    entity_manager_->destroy_entity(entity);
  }

  void update(float delta_time) override {
    //Will have more here as more comes online
    system_manager_->update(delta_time);
  }

  void process_events() override {
    //TODO Requires event manager system
  }

  [[nodiscard]] bool is_alive(Entity entity) const override {
    return entity_manager_->is_alive(entity);
  }

  template <typename T>
  void add_component(Entity entity, const T& component) {
    component_manager_->template add_component<T>(entity, component);
    Signature signature = entity_manager_->get_signature(entity.id);
    signature.set(component_manager_->template get_component_type<T>());
    entity_manager_->set_signature(entity.id, signature);
  }

  template <typename T>
  void remove_component(Entity entity) {
    component_manager_->template remove_component<T>(entity);
    Signature signature = entity_manager_->get_signature(entity.id);
    signature.reset(component_manager_->template get_component_type<T>());
    entity_manager_->set_signature(entity.id, signature);
  }

  template <typename T>
  T& get_component(Entity entity) {
    return component_manager_->template get_component<T>(entity);
  }

  template <typename T>
  [[nodiscard]] bool has_component(Entity entity) const {
    return component_manager_->template has_component<T>(entity);
  }

 private:
  std::shared_ptr<EntityMgr> entity_manager_;
  std::shared_ptr<ComponentMgr> component_manager_;
  std::shared_ptr<SystemMgr> system_manager_;
};

}  // namespace sb::ecs
#endif //GAMEWORLD_H
