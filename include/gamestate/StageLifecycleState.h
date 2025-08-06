//
// Created by Jace Shultz on 8/5/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
* @file StageLifecycleState.h
* @ingroup gamestate
* @brief Defines the lifecycle states for a game stage (level).
*
* @author Jace Shultz
* @date 8/5/2025
*/
#ifndef STAGESTATE_H
#define STAGESTATE_H
namespace sb::gamestate {

/**
* @enum StageLifecycleState
* @ingroup gamestate
* @brief Represents the current lifecycle state of an active game stage.
*
* This enum enables game systems and external controllers (such as the UI or main loop)
* to track the progression of a stage. It also allows stage transitions such as quitting,
* restarting, or ending the game.
*/
enum class StageLifecycleState {
  NotStarted,      /**< Stage has been initialized but gameplay has not yet begun */
  Active,          /**< Stage is actively running and updating systems */
  Paused,          /**< Stage is temporarily suspended (e.g., pause menu open) */
  ResetRequested,  /**< A reset has been requested; the level should be reloaded */
  Completed,       /**< Stage was successfully completed by the player */
  GameOver,        /**< The player has lost and has no lives remaining */
  Quit             /**< The player manually exited the stage (e.g., quit to menu) */
};

}
#endif //STAGESTATE_H
