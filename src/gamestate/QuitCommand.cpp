//
// Created by Jace Shultz on 7/21/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/gamestate/QuitCommand.h"
#include "../../include/ecs/components/Components.h"

const std::string sb::gamestate::QuitCommand::name_ = "QuitCommand";

void sb::gamestate::QuitCommand::apply(const std::shared_ptr<GameWorld> world) {
  world->request_exit();
  log::Logger::info("[Command Applied]: " + name_);
}