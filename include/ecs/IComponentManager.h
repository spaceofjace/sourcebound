//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef ICOMPONENTMANAGER_H
#define ICOMPONENTMANAGER_H

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "../logger/sinks/ConsoleSink.h"
#include "DenseComponentArray.h"
#include "Entity.h"
#include "IComponentArray.h"

namespace sb::ecs {
class IComponentManager {
public:
  virtual ~IComponentManager() = default;

  IComponentManager() = default;
  IComponentManager(const IComponentManager&) = delete;
  IComponentManager& operator=(const IComponentManager&) = delete;
  IComponentManager(IComponentManager&&) = delete;
  IComponentManager& operator=(IComponentManager&&) = delete;

  // This is the only non-templated function
  virtual void entity_destroyed(Entity entity) = 0;
};

class ComponentManager final : IComponentManager {
 public:

  template <typename T>
  void register_component() {
    auto type_index = std::type_index(typeid(T));
    if (component_types_.find(type_index) != component_types_.end()) {
      log::Logger::error("[ComponentManager] Component type already registered!");
      return;
    }
    component_types_[type_index] = next_component_type_;
    component_arrays_[type_index] = std::make_shared<DenseComponentArray<T>>();
    ++next_component_type_;
  }

  template <typename T>
  void add_component(Entity entity, const T& component) {
    if (ComponentType component_type; !try_get_component_type<T>(component_type)) {
      log::Logger::error("[ComponentManager] Tried to add unregistered component type.");
      return;
    }

    if (has_component<T>(entity)) {
      log::Logger::warn("[ComponentManager] Component already exists for this entity.");
      return;
    }

    get_component_array<T>()->insert_data(entity, component);
  }

  template <typename T>
  void remove_component(Entity entity) {
    if (!has_component<T>(entity)) {
      log::Logger::warn("[ComponentManager] Component doesn't exist for this entity.");
      return; // safe to skip
    }

    auto component_array = get_component_array<T>();
    component_array->remove_data(entity);
  }

  template <typename T>
  T& get_component(Entity entity) {
    if (!has_component<T>(entity)) {
      sb::log::Logger::error("[ComponentManager] Component doesn't exist for this entity.");

      // Throwing as this particular scenario could lead to very bad behavior
      throw std::runtime_error("[ComponentManager] Tried to get nonexistent component.");
    }

    auto component_array = get_component_array<T>();
    return component_array->get_data(entity);
  }

  template <typename T>
  [[nodiscard]] bool has_component(Entity entity) const {
    auto component_array = get_component_array<T>();
    return component_array->has_data(entity);
  }

  void entity_destroyed(const Entity entity) override {
    for (auto& [type_index, component_array] : component_arrays_) {
      component_array->entity_destroyed(entity);
    }
  }

  template <typename T>
  [[nodiscard]] ComponentType get_component_type() const {
    ComponentType component_type;

    if (!try_get_component_type<T>(component_type)) {
      log::Logger::error("[ComponentManager] Failed to get component type.");
      return static_cast<ComponentType>(-1);
    }

    return component_type;
  }

  template <typename T>
  [[nodiscard]] bool try_get_component_type(ComponentType& component_type) const {
    auto type_index = std::type_index(typeid(T));
    auto type = component_types_.find(type_index);

    if (type == component_types_.end()) {
      log::Logger::warn("[ComponentManager] Tried to get unregistered component type.");
      return false;
    }

    component_type = type->second;
    return true;
  }


 private:
  template <typename T>
  DenseComponentArray<T>* get_component_array() {
    return get_component_array_from_map<T>(component_arrays_);
  }

  template <typename T>
  const DenseComponentArray<T>* get_component_array() const {
    return get_component_array_from_map<T>(component_arrays_);
  }

  template <typename T, typename MapType>
  static auto get_component_array_from_map(MapType& map) {
    const auto type_index = std::type_index(typeid(T));
    const auto it = map.find(type_index);

    if (it == map.end()) {
      log::Logger::error("[ComponentManager] Component type not registered before use.");
    } else if (!it->second) {
      log::Logger::error("[ComponentManager] Component storage pointer is null.");
    }

    // allows for code reuse for const and non-const contexts
    using PointerType = std::conditional_t<std::is_const_v<MapType>, const DenseComponentArray<T>,
                                           DenseComponentArray<T>>;
    return static_cast<PointerType*>(it->second.get());
  }

 private:
  std::unordered_map<std::type_index, ComponentType> component_types_;
  std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> component_arrays_;
  ComponentType next_component_type_ = 0;
};

} // namespace sb::ecs
#endif //ICOMPONENTMANAGER_H
