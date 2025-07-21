//
// Created by Jace Shultz on 7/21/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file QuitCommand.h
 * @ingroup gamestate
 * @brief Defines a command that requests the game world to initiate shutdown.
 *
 * Used to signal that the main game loop should exit, typically triggered by input bindings
 * (e.g., Escape key) or system events (e.g., SDL_EVENT_QUIT). This command pattern allows
 * input-driven exit behavior without exposing window internals.
 * 
 * @author Jace Shultz
 * @date 7/21/2025
 */
#ifndef QUITCOMMAND_H
#define QUITCOMMAND_H
#include "GameWorld.h"
#include "ICommand.h"
namespace sb::gamestate {
/**
 * @class QuitCommand
 * @ingroup gamestate
 * @brief Represents a command to signal the game should exit.
 *
 * Typically triggered via Escape key or system events like SDL_EVENT_QUIT.
 * Calls GameWorld::request_exit to notify the main loop to terminate cleanly.
 */
class QuitCommand final : public ICommand {
public:
/**
 * @brief Returns the name identifier of the command.
 * @return String representing the command name.
 */
  [[nodiscard]] std::string name() const override {return name_;}

  QuitCommand() = default;

/**
   * @brief Applies the quit logic to the game world.
   * @param world The active game world instance.
   */
  void apply(std::shared_ptr<GameWorld> world) override;

private:
  static const std::string name_;
};
}// namespace sb::gamestate
#endif //QUITCOMMAND_H
