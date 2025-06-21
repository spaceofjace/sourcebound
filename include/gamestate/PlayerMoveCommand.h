//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file PlayerMoveCommand.h
 * @ingroup gamestate
 * @brief A command for triggering player (paddle) movement
 */

#ifndef PLAYERMOVECOMMAND_H
#define PLAYERMOVECOMMAND_H
#include "GameWorld.h"
#include "ICommand.h"
namespace sb::gamestate {
/**
 * @class PlayerMoveCommand
 * @ingroup gamestate
 * @brief Represents a command to apply player movement
 *
 * A command representing player movement; right now, this is pretty tightly coupled to the game
 * I am building, but in the future, I would love to extract this into a templated style command
 * that can indicate its intended target.  (Would be useful for "enemy" movement, for instance)
 */
class PlayerMoveCommand final : public ICommand {
 public:
  [[nodiscard]] std::string name() const override {return name_;}
  PlayerMoveCommand(const float x_velocity, const float y_velocity)
    : x_velocity_(x_velocity), y_velocity_(y_velocity) {}
  void apply(std::shared_ptr<GameWorld> world) override;

  //mostly exposed for testability, but may have other usage
  [[nodiscard]] float get_x() const { return x_velocity_; }
  [[nodiscard]] float get_y() const { return y_velocity_; }
private:
  float x_velocity_;
  float y_velocity_;
  static const std::string name_;
};
}// namespace sb::gamestate

#endif //PLAYERMOVECOMMAND_H
