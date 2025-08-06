//
// Created by Jace Shultz on 7/31/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file LaunchBallCommand.h
 * @ingroup gamestate
 * @brief Defines a command that launches the ball from a held state into motion.
 *
 * This command is used to transition one or more Ball entities from their pre-launch state
 *
 * @author Jace Shultz
 * @date 7/31/2025
 */
#ifndef LAUNCHBALLCOMMAND_H
#define LAUNCHBALLCOMMAND_H

#include "GameWorld.h"
#include "ICommand.h"

namespace sb::gamestate {
/**
 * @class LaunchBallCommand
 * @ingroup gamestate
 * @brief Represents a command that detaches the ball from the paddle and sets it in motion.
 *
 * This command applies an upward Direction to all Ball entities that:
 * - Have a Ball component
 * - Have a Direction component
 * - Have a PositionFollower component (indicating they are stuck to the paddle)
 *
 * It removes the PositionFollower component to allow the Physics system to take control.
 */
class LaunchBallCommand final : public ICommand {
public:
  LaunchBallCommand() = default;

  /**
  * @brief Returns the name identifier of the command.
  * @return String representing the command name ("LaunchBallCommand").
  */
  [[nodiscard]] std::string name() const override {return name_;}

  /**
  * @brief Applies the launch logic to the game world.
  * @param world The active GameWorld instance.
  *
  * Removes PositionFollower from all Ball entities and assigns them an initial upward direction.
  */
  void apply(std::shared_ptr<GameWorld> world) override;

private:
  static const std::string name_;
};
}// namespace sb::gamestate
#endif //LAUNCHBALLCOMMAND_H
