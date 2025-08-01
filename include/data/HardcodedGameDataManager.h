//
// Created by Jace Shultz on 7/25/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file HardcodedGameDataManager.h
 * @ingroup Data
 * @brief Provides a hardcoded implementation of game data configuration and level management.
 *
 * This class supplies static `LevelData` for the game using pre-initialized values,
 * acting as a temporary stand-in until external JSON or asset-based loading is available.
 * Also includes fallback logic in case an invalid level ID is requested.
 *
 * Intended primarily for development and testing.
 *
 * @author Jace Shultz
 * @date 7/25/2025
 */
#ifndef HARDCODEDGAMEDATAMANAGER_H
#define HARDCODEDGAMEDATAMANAGER_H
#include <unordered_map>

#include "IGameDataManager.h"
namespace sb::data {
class HardcodedGameDataManager final: public IGameDataManager {
public:
  /**
 * @brief Special level ID used as a fallback when a requested level cannot be found.
 *
 * This ID should correspond to a minimal or safe level definition stored in the manager,
 * and is returned when an invalid or missing level ID is requested.
 */
  static constexpr int kFallbackLevelId = -1;

  HardcodedGameDataManager();
  ~HardcodedGameDataManager() override = default;

  [[nodiscard]] const LevelData& get_current_level_data() const override;

  /**
  * @brief Loads predefined hardcoded levels into the internal map.
  *
  * This implementation does not load data from disk; instead, it populates a known
  * set of levels directly in memory for development and prototyping purposes.
  *
  * @param data_path Unused in this implementation. Present for interface compatibility.
  * @return True if levels were populated successfully.
  */
  bool load_config(const std::string& data_path) override;
  void set_current_level(int level_id) override;

private:
  int current_level_id_ = 0;
  std::unordered_map<int, LevelData> levels_;
};
} // namespace sb::data
#endif //HARDCODEDGAMEDATAMANAGER_H
