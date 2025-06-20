//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file IInputHandler.h
 * @ingroup input
 * @brief An abstraction representing some source of player inputs (keyboard, mouse, controller, etc)
 */

/**
 * @defgroup input Input
 * @brief Module wrapping input polling, input bindings, and all the associated translations to
 * commands.
 */

#ifndef IINPUTHANDLER_H
#define IINPUTHANDLER_H
#include <memory>

#include "IEventSource.h"
#include "IInputNormalizer.h"
namespace sb::input {
/**
 * @class IInputHandler
 * @ingroup input
 * @brief Interface for polling device input, translating them to input codes, and then passing them
 * on to an input normalizer (which translates them to actions)
 */
class IInputHandler {
public:
  virtual ~IInputHandler() = default;
  virtual void poll_inputs() = 0;
};

} // namespace sb::input
#endif //IINPUTHANDLER_H
