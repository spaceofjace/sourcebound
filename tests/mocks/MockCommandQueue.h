//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef MOCKCOMMANDQUEUE_H
#define MOCKCOMMANDQUEUE_H

#include <gmock/gmock.h>
#include "../../include/gamestate/ICommandQueue.h"
#include "../../include/gamestate/GameWorld.h"

using namespace sb::gamestate;

struct MockCommandQueue : ICommandQueue {
  MOCK_METHOD(void, enqueue, (std::unique_ptr<ICommand> command), (override));
  MOCK_METHOD(void, clear, (), (override));
  MOCK_METHOD(void, process,
    ((std::shared_ptr<GameWorld>)),
    (override));
};

#endif // MOCKCOMMANDQUEUE_H
