//
// Created by Jace Shultz on 6/20/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../include/input/InputNormalizer.h"
#include "../../../include/gamestate/PlayerMoveCommand.h"
#include "mocks/MockCommandQueue.h"
#include "mocks/MockBindingMap.h"

using namespace sb::input;
using namespace sb::gamestate;
using ::testing::_;
using ::testing::Return;
using ::testing::Pointee;
using ::testing::Truly;

TEST(InputNormalizerTest, EnqueuesMoveLeftWithNegativeValue) {
  auto mock_queue = std::make_shared<MockCommandQueue>();
  auto mock_bindings = std::make_shared<MockBindingMap>();
  InputNormalizer normalizer(mock_bindings, mock_queue);

  EXPECT_CALL(*mock_bindings, resolve(InputCode::FromKeyboard(SDL_SCANCODE_LEFT)))
      .WillOnce(Return(InputAction::MoveLeft));

  EXPECT_CALL(*mock_queue, enqueue(Truly([](const std::unique_ptr<ICommand>& cmd) {
    auto* move_cmd = dynamic_cast<const PlayerMoveCommand*>(cmd.get());
    return move_cmd && move_cmd->get_x() == -1.0f && move_cmd->get_y() == 0.0f;
  })));

  normalizer.normalize_input(InputCode::FromKeyboard(SDL_SCANCODE_LEFT), 1.0f);
}

TEST(InputNormalizerTest, EnqueuesMoveRightWithPositiveValue) {
  auto mock_queue = std::make_shared<MockCommandQueue>();
  auto mock_bindings = std::make_shared<MockBindingMap>();
  InputNormalizer normalizer(mock_bindings, mock_queue);

  EXPECT_CALL(*mock_bindings, resolve(InputCode::FromKeyboard(SDL_SCANCODE_RIGHT)))
      .WillOnce(Return(InputAction::MoveRight));

  EXPECT_CALL(*mock_queue, enqueue(Truly([](const std::unique_ptr<ICommand>& cmd) {
    auto* move_cmd = dynamic_cast<const PlayerMoveCommand*>(cmd.get());
    return move_cmd && move_cmd->get_x() == 1.0f && move_cmd->get_y() == 0.0f;
  })));

  normalizer.normalize_input(InputCode::FromKeyboard(SDL_SCANCODE_RIGHT), 1.0f);
}

TEST(InputNormalizerTest, IgnoresNoneAction) {
  auto mock_queue = std::make_shared<MockCommandQueue>();
  auto mock_bindings = std::make_shared<MockBindingMap>();
  InputNormalizer normalizer(mock_bindings, mock_queue);

  EXPECT_CALL(*mock_bindings, resolve(InputCode::FromKeyboard(SDL_SCANCODE_W)))
      .WillOnce(Return(InputAction::None));

  EXPECT_CALL(*mock_queue, enqueue(_)).Times(0);

  normalizer.normalize_input(InputCode::FromKeyboard(SDL_SCANCODE_W), 1.0f);
}