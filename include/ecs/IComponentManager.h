//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef ICOMPONENTMANAGER_H
#define ICOMPONENTMANAGER_H

#ifndef NDEBUG
#include <cassert>
#endif

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "Entity.h"
#include "IComponentArray.h"
#include "DenseComponentArray.h"

class IComponentManager {
public:
  virtual ~IComponentManager() = default;

  IComponentManager() = default;
  IComponentManager(const IComponentManager&) = delete;
  IComponentManager& operator=(const IComponentManager&) = delete;
  IComponentManager(IComponentManager&&) = delete;
  IComponentManager& operator=(IComponentManager&&) = delete;

  // This is the only usable non-templated function
  virtual void entity_destroyed(Entity entity) = 0;
};

class ComponentManager : IComponentManager {
 public:
  ComponentManager() = default;
  ComponentManager(const ComponentManager&) = delete;
  ComponentManager& operator=(const ComponentManager&) = delete;
  ComponentManager(ComponentManager&&) = delete;
  ComponentManager& operator=(ComponentManager&&) = delete;

  template <typename T>
  void register_component() {
    auto type_index = std::type_index(typeid(T));
    if (component_types_.find(type_index) != component_types_.end()) {

      // TODO: Will need to replace test with logging functionality once added
      assert(false && "Component type already registered!");

      return;
    }
    component_types_[type_index] = next_component_type_;
    component_arrays_[type_index] = std::make_shared<DenseComponentArray<T>>();
    ++next_component_type_;
  }

  template <typename T>
  void add_component(Entity entity, const T& component) {

    // TODO: Will need to replace test with logging functionality once added
    assert(!has_component<T>(entity) && "Component already exists for this entity.");

    auto component_array = get_component_array<T>();
    component_array->insert_data(entity, component);
  }

  template <typename T>
  void remove_component(Entity entity) {

    // TODO: Will need to replace test with logging functionality once added
    assert(has_component<T>(entity) && "Trying to remove component that doesn't exist.");

    auto component_array = get_component_array<T>();
    component_array->remove_data(entity);
  }

  template <typename T>
  T& get_component(Entity entity) {

    // TODO: Will need to replace test with logging functionality once added
    assert(has_component<T>(entity) && "Attempted to get a component that doesn't exist.");

    auto component_array = get_component_array<T>();
    return component_array->get_data(entity);
  }

  template <typename T>
  [[nodiscard]] bool has_component(Entity entity) const {
    auto component_array = get_component_array<T>();
    return component_array->has_data(entity);
  }

  void entity_destroyed(const Entity entity) override{
    for (auto& [type_index, component_array] : component_arrays_) {
      component_array->entity_destroyed(entity);
    }
  }

 private:
  using ComponentType = std::size_t;

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

    // TODO: Will need to replace test with logging functionality once added
    assert(it != map.end() && "Component type not registered before use.");
    assert(it->second && "Component storage pointer is null.");

    //allows for code reuse for const and non-const contexts
    using PointerType = std::conditional_t<std::is_const_v<MapType>,
                                           const DenseComponentArray<T>,
                                           DenseComponentArray<T>>;
    return static_cast<PointerType*>(it->second.get());
  }

  std::unordered_map<std::type_index, ComponentType> component_types_;
  std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> component_arrays_;
  ComponentType next_component_type_ = 0;
};
#endif //ICOMPONENTMANAGER_H
