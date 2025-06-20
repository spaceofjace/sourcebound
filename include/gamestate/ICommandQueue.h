//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file ICommandQueue.h
 * @ingroup gamestate
 * @brief A lightweight wrapper for queuing commands
 */

#ifndef ICOMMANDQUEUE_H
#define ICOMMANDQUEUE_H
#include <memory>

#include "ICommand.h"

namespace sb::gamestate {
using ecs::ComponentManager;
/**
 * @class ICommandQueue
 * @ingroup gamestate
 * @brief Functions as a communication bus for actions enqueued into the game world
 *
 * Built as an interface for enhanced testability, and to possible support other versions of
 * queuing such as a priority queue.
 */
class ICommandQueue {
public:
  virtual ~ICommandQueue() = default;

  /**
   * @brief Enqueue a command based to be executed in the next frame, primarily from player inputs
   *
   * @param command A high-level action intended to be executed in the engine
   */
  virtual void enqueue(std::unique_ptr<ICommand> command) = 0;

  /**
   * @brief Clears all queued commands. Typically used at the start of a frame.
   */
  virtual void clear() = 0;

  /**
   * @brief Processes or applies all current commands to the game world.
   * @param world The world in which to apply command state effects
   */
  virtual void process(std::shared_ptr<GameWorld<IEntityManager, ComponentManager, ISystemManager,
    ICommandQueue>> world) = 0;
};
} // namespace sb::gamestate
#endif //ICOMMANDQUEUE_H
