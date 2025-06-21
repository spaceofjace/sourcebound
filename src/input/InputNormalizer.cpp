//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/input/InputNormalizer.h"

#include "../../include/gamestate/PlayerMoveCommand.h"
void sb::input::InputNormalizer::normalize_input(const InputCode& code, float value) {
  auto action = this->binding_map_->resolve(code);

  switch (action) {

    case InputAction::MoveLeft: {
      //negative vector == left direction
      auto move_left_command = std::make_unique<gamestate::PlayerMoveCommand>(value * -1, 0);
      command_queue_->enqueue(move(move_left_command));
      break;
    }
    case InputAction::MoveRight: {
      auto move_right_command = std::make_unique<gamestate::PlayerMoveCommand>(value, 0);
      command_queue_->enqueue(move(move_right_command));
      break;
    }
    case InputAction::None:
    default:
      break;
  }
}