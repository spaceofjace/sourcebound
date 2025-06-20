//
// Created by Jace Shultz on 6/20/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/gamestate/CommandQueue.h"

void sb::gamestate::CommandQueue::enqueue(std::unique_ptr<ICommand> command) {
  commands_.push_back(std::move(command));
}

void sb::gamestate::CommandQueue::clear() {
  commands_.clear();
}
void sb::gamestate::CommandQueue::process( const std::shared_ptr<GameWorld<IEntityManager,
    ComponentManager, ISystemManager, ICommandQueue>> world) {

  for (const auto& command : commands_) {
    command->apply(world);
  }
}