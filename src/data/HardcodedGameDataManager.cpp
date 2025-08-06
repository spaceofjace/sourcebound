//
// Created by Jace Shultz on 7/25/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/data/HardcodedGameDataManager.h"

#include "../../include/logger/Logger.h"

sb::data::HardcodedGameDataManager::HardcodedGameDataManager() : current_level_id_(kFallbackLevelId) {
  LevelData level_data;
  level_data.level_id = kFallbackLevelId;
  level_data.level_name = "Fallback";
  levels_.emplace(kFallbackLevelId, level_data);
}

const sb::data::LevelData& sb::data::HardcodedGameDataManager::get_current_level_data() const {
  return levels_.at(current_level_id_);
}

bool sb::data::HardcodedGameDataManager::load_config(const std::string& data_path) {
  // this version does not read from disk, so we just need to do the hardcoded version
  LevelData level1;

  level1.level_id = 1;
  level1.level_name = "Level1";

  // Set bricks
  level1.brick_layout.brick_size = {64.0f, 24.0f};
  level1.brick_layout.brick_margin = {4.0f, 4.0f};

  constexpr int cols = 8;

  // Top 2 rows empty
  level1.bricks.push_back(make_empty_row(cols));
  level1.bricks.push_back(make_empty_row(cols));

  // Rows 2–4 = alternating pattern
  for (int row_index = 2; row_index < 5; ++row_index) {
    std::vector<std::optional<BrickConfig>> row;
    row.reserve(cols);

    for (int col_index = 0; col_index < cols; ++col_index) {
      if ((row_index + col_index) % 2 == 0) {
        row.emplace_back(make_brick(1, rendering::Colors::green, level1.brick_layout));
      } else {
        row.emplace_back(make_brick(2, rendering::Colors::orange, level1.brick_layout));
      }
    }
    level1.bricks.push_back(row);
  }

  // Bottom 10 rows empty
  for (int i = 0; i < 10; ++i) {
    level1.bricks.push_back(make_empty_row(cols));
  }

  levels_.emplace(level1.level_id, level1);


  levels_.emplace(level1.level_id, level1);
  return true;
}
void sb::data::HardcodedGameDataManager::set_current_level(const int level_id) {
  const auto found_level = levels_.find(level_id);
  if (found_level != levels_.end()) {
    current_level_id_ = level_id;
  } else {
    current_level_id_ = kFallbackLevelId;
    log::Logger::error(
        "[HardcodedDataManager] Could not find level with id: " + std::to_string(level_id) +
        ". Falling back to level ID: " + std::to_string(kFallbackLevelId));
  }
}
sb::data::BrickConfig sb::data::HardcodedGameDataManager::make_brick(
    int hits, const rendering::Color& color, const BrickLayoutConfig& layout) {
  BrickConfig config;
  config.hits = HitsRequired{hits};
  config.visual = RenderableSimpleShape{color, ecs::SimpleShapeType::Rectangle, true};
  config.collider = BoxCollider{layout.brick_size.x, layout.brick_size.y, 0.0F, 0.0F,
    ecs::CollisionBehavior::Bounce | ecs::CollisionBehavior::Trigger};
  config.triggers = ecs::Triggerable { { ecs::TriggerType::Hit }};
  config.score_value = ecs::ScoreValue{hits * 100}; //for now, just a static multiplier

  return config;
}
std::vector<std::optional<sb::data::BrickConfig>>
sb::data::HardcodedGameDataManager::make_empty_row(const int num_cols) {
  std::vector<std::optional<BrickConfig>> row;
  for (int i = 0; i < num_cols; ++i) {
    row.emplace_back(std::nullopt);
  }
  return row;
}