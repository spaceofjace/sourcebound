//
// Created by Jace Shultz on 5/8/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file GameWorld.h
 * @ingroup gamestate
 * @brief Defines the concrete game world used by the main loop.
 */

#ifndef GAMEWORLD_H
#define GAMEWORLD_H
#include <memory>

#include "../ecs/Entity.h"
#include "IGameWorld.h"
#include "../ecs/ecs_config.h"

//These includes are needed for the templated methods
#include "../ecs/IComponentManager.h"
#include "../ecs/IEntityManager.h"
#include "../ecs/ISystemManager.h"
#include "ICommandQueue.h"

namespace sb::gamestate {
using ecs::Entity;
using ecs::Signature;
/**
 * @class GameWorld
 * @ingroup gamestate
 * @brief Encapsulates the game state and manages ECS coordination, system updates, and entity access.
 */
class GameWorld final : public IGameWorld, public std::enable_shared_from_this<GameWorld> {
 public:
  GameWorld() = default;
  ~GameWorld() override = default;
  GameWorld(const GameWorld&) = delete;
  GameWorld& operator=(const GameWorld&) = delete;
  GameWorld(GameWorld&&) = delete;
  GameWorld& operator=(GameWorld&&) = delete;

  GameWorld(std::shared_ptr<ecs::IEntityManager> entity_manager,
          std::shared_ptr<ecs::ComponentManager> component_manager,
          std::shared_ptr<ecs::ISystemManager> system_manager,
          std::shared_ptr<ICommandQueue> cmd_queue)
    : entity_manager_(std::move(entity_manager)),
      component_manager_(std::move(component_manager)),
      system_manager_(std::move(system_manager)),
      cmd_queue_(std::move(cmd_queue)) {}

  void initialize(std::shared_ptr<rendering::IRenderer> renderer) override;
  Entity create_entity() override;
  [[nodiscard]] std::vector<Entity> get_entities_with_signature(
      const ecs::Signature& target_signature) const override;
  void destroy_entity(Entity entity) override;
  void step(float delta_time) override;
  void update(float delta_time) override;
  void process_events() override;
  [[nodiscard]] bool is_alive(Entity entity) const override;
  void load_level(const data::LevelData& level_data) override;
  void unload_level() override;

  [[nodiscard]] bool should_exit() const override;
  void request_exit() override;

  template <typename T>
  void register_component() const {
    component_manager_->register_component<T>();
  }

  template <typename T>
  void add_component(Entity entity, const T& component) {
    component_manager_->add_component<T>(entity, component);
    Signature signature = entity_manager_->get_signature(entity.id);
    signature.set(component_manager_->get_component_type<T>());
    entity_manager_->set_signature(entity.id, signature);
    system_manager_->entity_signature_changed(entity, signature);
  }

  template <typename T>
  void remove_component(Entity entity) {
    component_manager_->remove_component<T>(entity);
    Signature signature = entity_manager_->get_signature(entity.id);
    signature.reset(component_manager_->get_component_type<T>());
    entity_manager_->set_signature(entity.id, signature);
    system_manager_->entity_signature_changed(entity, signature);
  }

  template <typename T>
  std::size_t get_component_type() {
    return component_manager_->get_component_type<T>();
  }

  template <typename T>
  T& get_component(Entity entity) {
    return component_manager_->get_component<T>(entity);
  }

  template <typename T>
  [[nodiscard]] bool has_component(Entity entity) const {
    return component_manager_->has_component<T>(entity);
  }

 private:
  std::shared_ptr<ecs::IEntityManager> entity_manager_;
  std::shared_ptr<ecs::ComponentManager> component_manager_;
  std::shared_ptr<ecs::ISystemManager> system_manager_;
  std::shared_ptr<ICommandQueue> cmd_queue_;
  bool should_exit_ = false;
};

}  // namespace sb::gamestate
#endif //GAMEWORLD_H
