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
  MOCK_METHOD(void, draw_rect, (const sb::math::Vec2& center, int width, int height,
    const sb::rendering::Color& color, bool filled), (override));
  MOCK_METHOD(void, draw_circle, (const sb::math::Vec2& center, int radius,
    const sb::rendering::Color& color, bool filled), (override));
};
#endif //MOCKRENDERER_H
