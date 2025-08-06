//
// Created by Jace Shultz on 7/21/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../../include/gamestate/QuitCommand.h"
#include "../../../include/gamestate/GameWorld.h"
#include "mocks/MockCommandQueue.h"
#include "mocks/MockComponentManager.h"
#include "mocks/MockEntityManager.h"
#include "mocks/MockSystemManager.h"
#include "gtest/gtest.h"

TEST(QuitCommandTest, CallsRequestExitOnGameWorld) {
  using namespace sb::gamestate;

  auto em = std::make_shared<MockEntityManager>();
  auto cm = std::make_shared<ComponentManager>();   //real component manager
  auto sm = std::make_shared<MockSystemManager>();
  auto cq = std::make_shared<MockCommandQueue>();
  auto gw = std::make_shared<GameWorld>(em, cm, sm, cq);

  EXPECT_FALSE(gw->get_stage_lifecycle_state() == sb::gamestate::StageLifecycleState::Quit);

  QuitCommand cmd;
  cmd.apply(gw);

  EXPECT_TRUE(gw->get_stage_lifecycle_state() == sb::gamestate::StageLifecycleState::Quit);
}