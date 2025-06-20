//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file InputAction.h
 * @ingroup input
 * @brief Defines a list of bindable player actions
 */

#ifndef INPUTACTION_H
#define INPUTACTION_H
namespace sb::input {
/**
 * @enum InputAction
 * @ingroup input
 * @brief Enum representing all available player input actions.
 *
 * For now, these are game specific; once there is a case for more than one game, it will make sense
 * to shift these to a string mapping instead and probably manage command intent generation through a
 * game specific factory.
 */
enum class InputAction {
  None = 0,
  MoveLeft,
  MoveRight,
  LaunchBall,
  PauseGame,
};
} // namespace sb::input
#endif //INPUTACTION_H
