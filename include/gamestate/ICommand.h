//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file ICommand.h
 * @ingroup gamestate
 * @brief An abstract representation of an action to be executed in the game engine
 */

#ifndef ICOMMAND_H
#define ICOMMAND_H
#include <string>

#include "../ecs/ISystemManager.h"

namespace sb::gamestate {
class GameWorld;
using ecs::ISystemManager;
//forward declaration to prevent circular includes; only needed for GameWorld reference
class ICommandQueue;

/**
 * @class ICommand
 * @ingroup gamestate
 * @brief Represents a queued command derived from player input.
 *
 * An ICommand represents the "what the player wants to do" abstraction,
 * decoupling input from actual behavior. These are typically created by input
 * systems and executed later during the game loop.
 */
class ICommand {
public:
  virtual ~ICommand() = default;

  /**
   * @brief Returns a descriptive name of the intent (for debugging/logging).
   */
  [[nodiscard]] virtual std::string name() const = 0;

  /**
   * @brief Executes this intent against the game world.
   * @param world The world to execute the commands against
   */
  virtual void apply(std::shared_ptr<GameWorld> world) = 0;
};

}  // namespace sb::gamestate
#endif //ICOMMAND_H
