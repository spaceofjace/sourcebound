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
  LevelData level1 { 1,"Level1",800, 600};
  levels_.emplace(level1.level_id, level1);
  return true;
}
void sb::data::HardcodedGameDataManager::set_current_level(const int level_id) {

  const auto found_level = levels_.find(level_id);
  if (found_level != levels_.end()) {
    current_level_id_ = level_id;
  }
  else {
    current_level_id_ = kFallbackLevelId;
    log::Logger::error("[HardcodedDataManager] Could not find level with id: "
      + std::to_string(level_id) + ". Falling back to level ID: "
      + std::to_string(kFallbackLevelId));
  }
}