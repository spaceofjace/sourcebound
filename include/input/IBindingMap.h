//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file IBindingMap.h
 * @ingroup input
 * @brief Represents a source of loadable and saveable bindings.
 */

#ifndef IBINDINGMAP_H
#define IBINDINGMAP_H
#include <string>

#include "InputAction.h"
#include "InputCode.h"
namespace sb::input {
/**
 * @class IBindingMap
 * @ingroup input
 * @brief An interface built to allow for bindings to be serializable from some source (presumably
 * on disk, but could theoretically include a network source or some other persistence)
 */
class IBindingMap {
public:
  virtual ~IBindingMap() = default;

  /// @brief Resolves an InputCode to its logical action.
  [[nodiscard]] virtual InputAction resolve(const InputCode& input) const = 0;

  /// @brief Binds a new input to an action name.
  virtual void bind(const InputCode& input, InputAction action) = 0;

  /// @brief Loads input bindings from persistent storage.
  virtual void load_bindings(const std::string& path) = 0;

  /// @brief Saves input bindings to persistent storage.
  virtual void save_bindings(const std::string& path) const = 0;
};

} // namespace sb::input
#endif //IBINDINGMAP_H
