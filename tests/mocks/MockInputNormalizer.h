//
// Created by Jace Shultz on 6/20/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//
#ifndef MOCKINPUTNORMALIZER_H
#define MOCKINPUTNORMALIZER_H

#include <gmock/gmock.h>
#include "../../../include/input/IInputNormalizer.h"

class MockInputNormalizer : public sb::input::IInputNormalizer {
public:
  MOCK_METHOD(void, normalize_input, (const sb::input::InputCode& code, float value), (override));
};

#endif // MOCKINPUTNORMALIZER_H