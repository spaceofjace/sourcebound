//
// Created by shult on 6/20/2025.
//

#include <gtest/gtest.h>

#include "../include/gamestate/CommandQueue.h"
#include "../include/gamestate/GameWorld.h"
#include "mocks/MockComponentManager.h"
#include "mocks/MockEntityManager.h"
#include "mocks/MockSystemManager.h"
#include "mocks/SimpleMockCommand.h"

TEST(CommandQueueTest, EnqueueAddsCommand) {
  auto queue = std::make_shared<CommandQueue>();
  auto cmd = std::make_unique<SimpleMockCommand>();
  auto* raw_ptr = cmd.get();  // Keep a reference to verify later

  queue->enqueue(std::move(cmd));

  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>();
  //unfortunately need concrete version of ComponentManager for now
  auto cm = std::make_shared<ComponentManager>();

  auto world = std::make_shared<GameWorld<IEntityManager, ComponentManager, ISystemManager,
    ICommandQueue>>(em, cm, sm, queue);

  queue->process(world);
  EXPECT_TRUE(raw_ptr->was_applied);
}

TEST(CommandQueueTest, ClearRemovesAllCommands) {
  auto queue = std::make_shared<CommandQueue>();
  queue->enqueue(std::make_unique<SimpleMockCommand>());
  queue->clear();

  auto em = std::make_shared<MockEntityManager>();
  auto sm = std::make_shared<MockSystemManager>();
  //unfortunately need concrete version of ComponentManager for now
  auto cm = std::make_shared<ComponentManager>();

  auto world = std::make_shared<GameWorld<IEntityManager, ComponentManager, ISystemManager,
    ICommandQueue>>(em, cm, sm, queue);

  // No command should run
  queue->process(world);  // Should not crash or apply anything
}