//
// Created by shult on 6/20/2025.
//

#include <gtest/gtest.h>
#include "../../../../include/input/HardcodedBindingMap.h"

using namespace sb::input;

TEST(HardcodedBindingMapTest, BindsAndResolvesCorrectly) {
  HardcodedBindingMap bindings;
  InputCode code = InputCode::FromKeyboard(SDL_SCANCODE_A);

  bindings.bind(code, InputAction::MoveLeft);
  auto result = bindings.resolve(code);

  EXPECT_EQ(result, InputAction::MoveLeft);
}

TEST(HardcodedBindingMapTest, ReturnsNoneForUnboundKey) {
  HardcodedBindingMap bindings;
  InputCode unbound = InputCode::FromKeyboard(SDL_SCANCODE_B);

  auto result = bindings.resolve(unbound);
  EXPECT_EQ(result, InputAction::None);
}

TEST(HardcodedBindingMapTest, LoadBindingsSetsDefaultKeys) {
  HardcodedBindingMap bindings;
  bindings.load_bindings("dummy_path");

  EXPECT_EQ(bindings.resolve(sb::input::InputCode::FromKeyboard(SDL_SCANCODE_LEFT)),
            InputAction::MoveLeft);
  EXPECT_EQ(bindings.resolve(sb::input::InputCode::FromKeyboard(SDL_SCANCODE_RIGHT)),
            InputAction::MoveRight);
  EXPECT_EQ(bindings.resolve(sb::input::InputCode::FromKeyboard(SDL_SCANCODE_SPACE)),
            InputAction::LaunchBall);
}