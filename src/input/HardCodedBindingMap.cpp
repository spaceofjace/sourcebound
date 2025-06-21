//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/input/HardcodedBindingMap.h"
sb::input::InputAction sb::input::HardcodedBindingMap::resolve(const InputCode& input) const {
  auto found_binding = this->bindings_.find(input);
  if (found_binding != this->bindings_.end()) {
    return found_binding->second;
  }

  return InputAction::None;
}
void sb::input::HardcodedBindingMap::bind(const InputCode& input, InputAction action) {
  bindings_[input] = action;
}

void sb::input::HardcodedBindingMap::load_bindings(const std::string& path) {
  bindings_.emplace(InputCode::FromKeyboard(SDL_SCANCODE_LEFT), InputAction::MoveLeft);
  bindings_.emplace(InputCode::FromKeyboard(SDL_SCANCODE_RIGHT), InputAction::MoveRight);
  bindings_.emplace(InputCode::FromKeyboard(SDL_SCANCODE_SPACE), InputAction::LaunchBall);
}
void sb::input::HardcodedBindingMap::save_bindings(const std::string& path) const {
  //no-op
}