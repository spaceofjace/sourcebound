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
  /**
  * @brief Constructs a BrickConfig with hitpoints, visual color, and collider size.
  *
  * This utility wraps the process of assembling a brick with defaulted component values
  * based on hit strength and a provided color. The resulting brick includes:
  * - `HitsRequired` set to the given value
  * - `RenderableSimpleShape` using the specified fill color and brick size
  * - `BoxCollider` matching the brick's dimensions with bounce and trigger enabled
  *
  * @param hits Number of hits required to break the brick.
  * @param color Fill color used for rendering the brick.
  * @param layout Reference to the current brick layout configuration (size, margins).
  * @return A complete BrickConfig instance with all relevant components populated.
  */
  static BrickConfig make_brick(int hits, const rendering::Color& color, const BrickLayoutConfig& layout);

  /**
  * @brief Creates a row of empty (nullopt) brick cells.
  *
  * Used to insert spacing or non-playable rows into the brick grid.
  * Typically applied to top or bottom padding rows or gaps in level design.
  *
  * @param num_cols Number of columns in the brick grid.
  * @return A vector of std::optional<BrickConfig>, all set to std::nullopt.
  */
  static std::vector<std::optional<BrickConfig>> make_empty_row(int num_cols);

  int current_level_id_ = 0;
  std::unordered_map<int, LevelData> levels_;
};
} // namespace sb::data
#endif //HARDCODEDGAMEDATAMANAGER_H
