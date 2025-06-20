//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file KeyboardHandler.h
 * @ingroup input
 * @brief Concrete implementation of the InputHandler interface for polling keyboard inputs
 */

#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include "BaseInputHandler.h"
namespace sb::input {

/**
 * @class KeyboardHandler
 * @ingroup input
 * @brief polls inputs through the SDL library and passes those on to a normalizer for translation
 * into game engine commands; later, this can imitate things like analog input though "holding" keys
 * down (slowly ramping speed)
 */
class KeyboardHandler final : BaseInputHandler {
public:
  KeyboardHandler(std::shared_ptr<IInputNormalizer> normalizer,
    std::shared_ptr<IEventSource> eventSource)
    : BaseInputHandler(std::move(normalizer), std::move(eventSource)) {}
  void poll_inputs() override;
};

} // namespace sb::input
#endif //KEYBOARDHANDLER_H
