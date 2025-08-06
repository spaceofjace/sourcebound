//
// Created by Jace Shultz on 7/25/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file IGameDataManager.h
 * @ingroup Data
 * @brief Interface for retrieving game configuration and player state data.
 *
 * This interface provides access to current level configuration (`LevelData`) and is designed to
 * be extensible to support dynamic game state, such as * player lives, score, and stage progression.
 *
 * Initially backed by hardcoded values for rapid prototyping. Later implementations may load from
 * JSON files or persistent player profiles.
 *
 * @note For now, only static configuration (LevelData) is implemented. Dynamic state methods may
 * be added in future milestones.
 *
 * @author Jace Shultz
 * @date 7/25/2025
 */

/**
 * @defgroup Data Game Data & Configuration
 * @brief Provides access to level configuration, player state, and runtime parameters.
 *
 * Includes level layout structs, data managers, and interfaces that expose persistent
 * and mutable game data to other systems.
 */

#ifndef IGAMEDATAMANAGER_H
#define IGAMEDATAMANAGER_H
#include "LevelData.h"
namespace sb::data {

/**
* @interface IGameDataManager
* @brief Provides access to the current level configuration and (eventually) player state.
*
* Intended to decouple gameplay systems from configuration sources. This interface allows systems
* to read level-specific data (like paddle speed, arena size, margins) without direct file access.
*
* In future expansions, this interface may also manage:
* - Player score and lives
* - Game progression state
* - Temporary buffs and modifiers
* - Save/load mechanics
*/
class IGameDataManager {
public:

  virtual ~IGameDataManager() = default;
  /**
  * @brief Returns the current level configuration data.
  * @return A reference to the active LevelData struct.
  */
  [[nodiscard]] virtual const LevelData& get_current_level_data() const = 0;

  /**
  * @brief Loads configuration from a file path.
  * @param data_path Path to the configuration file.
  * @return True if loaded successfully.
  */
  virtual bool load_config(const std::string& data_path) = 0;

  /**
  * @brief Sets the current level ID (for internal tracking or stage switching).
  * @param level_id The numeric ID of the active level.
  */
  virtual void set_current_level(int level_id) = 0;
};
} // namespace sb::data
#endif //IGAMEDATAMANAGER_H
