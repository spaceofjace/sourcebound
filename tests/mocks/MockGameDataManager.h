//
// Created by Jace Shultz on 7/25/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef MOCKGAMEDATAMANAGER_H
#define MOCKGAMEDATAMANAGER_H
#include "../../include/data/IGameDataManager.h"
#include "../../include/ecs/components/Components.h"

using namespace sb::ecs;

class MockGameDataManager : public sb::data::IGameDataManager {
public:
  MockGameDataManager() {
    fallback_level_.level_id = kFallbackLevelId;
    fallback_level_.level_name = "Fallback";
    fallback_level_.paddle_speed = 100.0f;
    fallback_level_.ball_speed = 150.0f;
    fallback_level_.wall_thickness = 5.0f;

    sb::data::BrickLayoutConfig layout;
    layout.brick_size = {20.0f, 10.0f};
    layout.brick_margin = {2.0f, 2.0f};

    // 2x1 brick layout
    fallback_level_.brick_layout = layout;

    fallback_level_.bricks = {
      {sb::data::BrickConfig{HitsRequired{1}, RenderableSimpleShape { }}},
      {sb::data::BrickConfig{HitsRequired{1}, RenderableSimpleShape { }}}
    };
  }

  bool load_config(const std::string&) override { return true; }

  void set_current_level(int /*level_id*/) override {
    // Do nothing – always fallback
  }

  [[nodiscard]] const sb::data::LevelData& get_current_level_data() const override {
    return fallback_level_;
  }

  void set_paddle_speed(float speed) { fallback_level_.paddle_speed = speed; }
  void set_ball_speed(float speed) { fallback_level_.ball_speed = speed; }

private:
  static constexpr int kFallbackLevelId = -1;
  sb::data::LevelData fallback_level_;
};

#endif //MOCKGAMEDATAMANAGER_H
