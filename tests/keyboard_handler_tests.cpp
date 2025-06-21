//
// Created by Jace Shultz on 6/20/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>
#include <memory>
#include "mocks/MockInputNormalizer.h"
#include "mocks/MockEventSource.h"
#include "../include/input/KeyboardHandler.h"
#include "SDL3/SDL_events.h"

using namespace sb::input;

TEST(KeyboardHandlerTest, PollInputs_TranslatesKeyDown) {
  auto mock_normalizer = std::make_shared<MockInputNormalizer>();
  auto mock_event_source = std::make_shared<MockEventSource>();

  SDL_Event event{};
  event.type = SDL_EVENT_KEY_DOWN;
  event.key.scancode = SDL_SCANCODE_RIGHT;
  event.key.repeat = false;

  EXPECT_CALL(*mock_event_source, poll_event(::testing::_))
    .WillOnce([&event](SDL_Event& e) {
      e = event;
      return true;
    })
    .WillOnce([](SDL_Event&) { return false; });

  EXPECT_CALL(*mock_normalizer, normalize_input(InputCode::FromKeyboard(SDL_SCANCODE_RIGHT), 1.0f))
    .Times(1);

  KeyboardHandler handler(mock_normalizer, mock_event_source);
  handler.poll_inputs();
}

TEST(KeyboardHandlerTest, PollInputs_TranslatesKeyUp) {
  auto mock_normalizer = std::make_shared<MockInputNormalizer>();
  auto mock_event_source = std::make_shared<MockEventSource>();

  SDL_Event event{};
  event.type = SDL_EVENT_KEY_UP;
  event.key.scancode = SDL_SCANCODE_LEFT;

  EXPECT_CALL(*mock_event_source, poll_event(::testing::_))
    .WillOnce([&event](SDL_Event& e) {
      e = event;
      return true;
    })
    .WillOnce([](SDL_Event&) { return false; });

  EXPECT_CALL(*mock_normalizer, normalize_input(InputCode::FromKeyboard(SDL_SCANCODE_LEFT), 0.0f))
    .Times(1);

  KeyboardHandler handler(mock_normalizer, mock_event_source);
  handler.poll_inputs();
}

TEST(KeyboardHandlerTest, PollInputs_IgnoresRepeatKeydown) {
  auto mock_normalizer = std::make_shared<MockInputNormalizer>();
  auto mock_event_source = std::make_shared<MockEventSource>();

  SDL_Event event{};
  event.type = SDL_EVENT_KEY_DOWN;
  event.key.scancode = SDL_SCANCODE_A;
  event.key.repeat = 1;

  EXPECT_CALL(*mock_event_source, poll_event(::testing::_))
    .WillOnce([&event](SDL_Event& e) {
      e = event;
      return true;
    })
    .WillOnce([](SDL_Event&) { return false; });

  EXPECT_CALL(*mock_normalizer, normalize_input(::testing::_, ::testing::_)).Times(0);

  KeyboardHandler handler(mock_normalizer, mock_event_source);
  handler.poll_inputs();
}

TEST(KeyboardHandlerTest, PollInputs_IgnoresOtherEventSource) {
  auto mock_normalizer = std::make_shared<MockInputNormalizer>();
  auto mock_event_source = std::make_shared<MockEventSource>();

  SDL_Event event{};
  event.type = SDL_EVENT_MOUSE_MOTION;

  EXPECT_CALL(*mock_event_source, poll_event(::testing::_))
    .WillOnce([&event](SDL_Event& e) {
      e = event;
      return true;
    })
    .WillOnce([](SDL_Event&) { return false; });

  EXPECT_CALL(*mock_normalizer, normalize_input(::testing::_, ::testing::_)).Times(0);

  KeyboardHandler handler(mock_normalizer, mock_event_source);
  handler.poll_inputs();
}

TEST(KeyboardHandlerTest, PollInputs_ExitsLoopOnQuitEvent) {
  auto mock_normalizer = std::make_shared<MockInputNormalizer>();
  auto mock_event_source = std::make_shared<MockEventSource>();

  SDL_Event quit_event{};
  quit_event.type = SDL_EVENT_QUIT;

  // Simulate only a quit event
  EXPECT_CALL(*mock_event_source, poll_event(::testing::_))
    .WillOnce([&quit_event](SDL_Event& e) {
      e = quit_event;
      return true;
    })
    .WillOnce([](SDL_Event&) { return false; });

  EXPECT_CALL(*mock_normalizer, normalize_input(::testing::_, ::testing::_)).Times(0);

  KeyboardHandler handler(mock_normalizer, mock_event_source);
  handler.poll_inputs();
}
