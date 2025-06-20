//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file HardcodedBindingMap.h
 * @ingroup input
 * @brief A preconfigured set of hardcoded bindings until serialization is available
 */

#ifndef HARDCODEDBINDINGMAP_H
#define HARDCODEDBINDINGMAP_H
#include <unordered_map>

#include "IBindingMap.h"
#include "InputAction.h"
#include "InputCode.h"
namespace sb::input {
/**
 * @class HardcodedBindingMap
 * @ingroup input
 * @brief A preconfigured set of hardcoded bindings to serve as a placeholder until the implementation
 * of serializable bindings.  Notably, "save" has no effect.
 *
 * Later, this will still be useful for testing.
 */
class HardcodedBindingMap : public IBindingMap {
public:
  HardcodedBindingMap() = default;

  /// Resolves an InputCode to its logical action.
  [[nodiscard]] InputAction resolve(const InputCode& input) const override;

  /// Binds a new input to an action name.
  void bind(const InputCode& input, InputAction action) override;

  /// Loads input bindings from persistent storage.
  void load_bindings(const std::string& path) override;

  /// Saves input bindings to persistent storage.
  void save_bindings(const std::string& path) const override;

private:
  std::unordered_map<InputCode, InputAction> bindings_;
};
} // namespace sb::input
#endif //HARDCODEDBINDINGMAP_H
