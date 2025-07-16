//
// Created by Jace Shultz on 7/16/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef MOCKRENDERER_H
#define MOCKRENDERER_H
#include <gmock/gmock.h>
#include "../../../include/rendering/IRenderer.h"

class MockRenderer final : public sb::rendering::IRenderer {
public:
  MOCK_METHOD(void, clear, (), (override));
  MOCK_METHOD(void, present, (), (override));
  MOCK_METHOD(void, draw_rect, (int, int, int, int, const sb::rendering::Color&, bool), (override));
  MOCK_METHOD(void, draw_circle, (int, int, int, const sb::rendering::Color&, bool), (override));
};
#endif //MOCKRENDERER_H
