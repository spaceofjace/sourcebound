//
// Created by Jace Shultz on 6/5/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>
#include "../include/input/InputCode.h"

using namespace sb::input;

TEST(InputCodeTest, RoundTripKeyboard) {
  InputCode original = InputCode::FromKeyboard(SDL_SCANCODE_A);
  std::string str = original.to_string();
  InputCode parsed = InputCode::from_string(str);

  EXPECT_EQ(parsed.device, original.device);
  EXPECT_EQ(parsed.kind, original.kind);
  EXPECT_EQ(std::get<SDL_Scancode>(parsed.data), std::get<SDL_Scancode>(original.data));
}

TEST(InputCodeTest, RoundTripGamepadAxis) {
  InputCode original = InputCode::FromGamepadAxis(SDL_GAMEPAD_AXIS_LEFTX);
  std::string str = original.to_string();
  InputCode parsed = InputCode::from_string(str);

  EXPECT_EQ(parsed.device, original.device);
  EXPECT_EQ(parsed.kind, original.kind);
  EXPECT_EQ(std::get<SDL_GamepadAxis>(parsed.data), std::get<SDL_GamepadAxis>(original.data));
}

TEST(InputCodeTest, InvalidFormatThrows) {
  EXPECT_THROW(InputCode::from_string("Gibberish"), std::invalid_argument);
  EXPECT_THROW(InputCode::from_string("Keyboard:"), std::invalid_argument);
}

TEST(InputCodeTest, UnknownScancodeThrows) {
  EXPECT_THROW(InputCode::from_string("Keyboard:NotARealKey"), std::invalid_argument);
}

TEST(InputCodeTest, ToStringIsCorrect) {
  InputCode code = InputCode::FromGamepadButton(SDL_GAMEPAD_BUTTON_START);
  EXPECT_EQ(code.to_string(), "GamepadButton:start");
}