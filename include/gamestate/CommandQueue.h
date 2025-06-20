//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file CommandQueue.h
 * @ingroup gamestate
 * @brief Concrete implementation of ICommandQueue
 */

#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H
#include "ICommandQueue.h"
namespace sb::gamestate {
using ecs::ComponentManager;
/**
 * @class CommandQueue
 * @ingroup gamestate
 * @brief A simple concrete implementation for enqueuing commands
 *
 * Built as an interface for enhanced testability, and to possible support other versions of
 * queuing such as a priority queue.
 */
class CommandQueue final : public ICommandQueue {
 public:
  void enqueue(std::unique_ptr<ICommand> command) override;
  void clear() override;
  void process(std::shared_ptr<GameWorld<IEntityManager, ComponentManager, ISystemManager,
    ICommandQueue>> world) override;
private:
  std::vector<std::unique_ptr<ICommand>> commands_;
};
} // namespace sb::gamestate
#endif //COMMANDQUEUE_H
