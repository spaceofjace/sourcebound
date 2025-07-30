//
// Created by Jace Shultz on 7/25/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef MOCKGAMEDATAMANAGER_H
#define MOCKGAMEDATAMANAGER_H
#include "../../include/data/IGameDataManager.h"

class MockGameDataManager : public sb::data::IGameDataManager {
public:
  MockGameDataManager() {
    fallback_level_.level_id = kFallbackLevelId;
    fallback_level_.level_name = "Fallback";
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
