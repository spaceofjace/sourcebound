//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file IInputNormalizer.h
 * @ingroup input
 * @brief An abstraction for translating raw inputs through bindings and into commands
 */

#ifndef IINPUTNORMALIZER_H
#define IINPUTNORMALIZER_H
#include "InputCode.h"
namespace sb::input {
/**
 * @class IInputNormalizer
 * @ingroup input
 * @brief Abstrcation designed to centralize the translation of raw inputs into some command
 * that can be understood by the engine.
 */
class IInputNormalizer {
public:
  virtual ~IInputNormalizer() = default;

  virtual void normalize_input(const InputCode& code, float value) = 0;
};
} // namespace sb::input
#endif //IINPUTNORMALIZER_H
