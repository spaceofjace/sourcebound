//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file InputNormalizer.h
 * @ingroup input
 * @brief Concrete implementation of InputNormalizer
 */

#ifndef INPUTNORMALIZER_H
#define INPUTNORMALIZER_H
#include <memory>
#include <utility>

#include "../gamestate/ICommandQueue.h"
#include "IBindingMap.h"
#include "IInputNormalizer.h"

/**
 * @class InputNormalizer
 * @ingroup input
 * @brief Concrete implementation of InputNormalizer that takes a binding map and uses it translate
 * raw inputs into commands that are understandable by the game engine
 */
namespace sb::input {
using gamestate::ICommandQueue;
class InputNormalizer final : public IInputNormalizer {
 public:
  InputNormalizer(std::shared_ptr<IBindingMap> binding_map,
                  std::shared_ptr<ICommandQueue> command_queue)
      : binding_map_(std::move(binding_map)), command_queue_(std::move(command_queue)) {
    binding_map_->load_bindings("bindings.json");
  }
  void normalize_input(const InputCode& code, float value) override;

 private:
  std::shared_ptr<IBindingMap> binding_map_;
  std::shared_ptr<ICommandQueue> command_queue_;
};
} //namespace sb::input
#endif //INPUTNORMALIZER_H
