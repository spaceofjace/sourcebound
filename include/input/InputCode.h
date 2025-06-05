//
// Created by Jace Shultz on 6/4/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file InputCode.h
 * @ingroup input
 * @brief  Defines a unified abstraction for all input bindings.
 * 
 * @author Jace Shultz
 * @date 6/4/2025
 */
#ifndef INPUTCODE_H
#define INPUTCODE_H

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>

#include <stdexcept>
#include <string>
#include <variant>

namespace sb::input {

/**
 * @enum  DeviceType
 * @ingroup input
 * @brief Identifies the physical input device used for a control.
 *
 * Used to distinguish between keyboard keys, mouse inputs, and gamepad controls.
 */
enum class DeviceType {
  Invalid = -1,
  Keyboard,
  Mouse,
  Gamepad
};

/**
 * @enum InputKind
 * @ingroup input
 * @brief Describes whether the input represents a button or analog axis.
 *
 * Used in combination with the device type to interpret the meaning of the variant payload.
 */
enum class InputKind {
  Invalid = -1,
  Button,
  Axis
};

/**
 * @typedef InputData
 * @ingroup input
 * @brief Variant container for raw input identifiers from different devices.
 *
 * Holds one of the following:
 * - SDL_Scancode (keyboard key)
 * - Uint8 (mouse button or axis)
 * - SDL_GamepadButton
 * - SDL_GamepadAxis
 *
 * Used as the underlying data representation for InputCode.
 */
using InputData = std::variant<
    SDL_Scancode,
    Uint8, // mouse_button or mouse_axis
    SDL_GamepadButton,
    SDL_GamepadAxis
>;

/**
 * @struct InputCode
 * @brief Represents a single user input binding
 *
 * Combines a device type (keyboard, mouse, or gamepad), an input kind (button vs. axis),
 * and a variant payload (`InputData`) that stores the appropriate device-specific code.
 *
 * DeviceType and InputKind will be set to Invalid if not initialized/parsed correctly.
 *
 * @see DeviceType
 * @see InputKind
 * @see InputData
 */
struct InputCode {
  DeviceType device{ DeviceType::Invalid };
  InputKind kind{ InputKind::Invalid };
  InputData data;

  InputCode() = default;

  static InputCode FromKeyboard(SDL_Scancode code) {
    return { DeviceType::Keyboard, InputKind::Button, code };
  }

  static InputCode FromMouseButton(Uint8 button) {
    return { DeviceType::Mouse, InputKind::Button, button };
  }

  static InputCode FromMouseAxis(Uint8 axis) {
    return { DeviceType::Mouse, InputKind::Axis, axis };
  }

  static InputCode FromGamepadButton(SDL_GamepadButton button) {
    return { DeviceType::Gamepad, InputKind::Button, button };
  }

  static InputCode FromGamepadAxis(SDL_GamepadAxis axis) {
    return { DeviceType::Gamepad, InputKind::Axis, axis };
  }

  [[nodiscard]] bool is_axis() const { return kind == InputKind::Axis; }

  [[nodiscard]] std::string to_string() const {
    const char* kind_str = (kind == InputKind::Axis) ? "Axis" : "Button";

    return std::visit([&](auto&& code_value) -> std::string {
      using T = std::decay_t<decltype(code_value)>;
      if constexpr (std::is_same_v<T, SDL_Scancode>) {
          const char* name = SDL_GetScancodeName(code_value);
          return std::string("Keyboard:") + (name ? name : "Unknown");
      } else if constexpr (std::is_same_v<T, Uint8>) {
          return std::string("Mouse") + kind_str + ":" + std::to_string(code_value);
      } else if constexpr (std::is_same_v<T, SDL_GamepadButton>) {
          const char* name = SDL_GetGamepadStringForButton(code_value);
          return std::string("GamepadButton:") + (name ? name : "Unknown");
      } else if constexpr (std::is_same_v<T, SDL_GamepadAxis>) {
          const char* name = SDL_GetGamepadStringForAxis(code_value);
          return std::string("GamepadAxis:") + (name ? name : "Unknown");
      } else {
          return "Unknown";
      }
    }, data);
  }

  static InputCode InputCode::from_string(const std::string& str) {
    const auto separator = str.find(':');
    if (separator == std::string::npos) {
      throw std::invalid_argument("Malformed InputCode string: " + str);
    }

    const std::string type_kind = str.substr(0, separator);
    const std::string value = str.substr(separator + 1);

    if (type_kind == "Keyboard") {
      const SDL_Scancode code = SDL_GetScancodeFromName(value.c_str());
      if (code == SDL_SCANCODE_UNKNOWN) {
        throw std::invalid_argument("Unknown scancode: " + value);
    }
      return FromKeyboard(code);
    }
    if (type_kind == "MouseButton") {
      return FromMouseButton(static_cast<Uint8>(std::stoi(value)));
    }
    if (type_kind == "MouseAxis") {
      return FromMouseAxis(static_cast<Uint8>(std::stoi(value)));
    }
    if (type_kind == "GamepadButton") {
      const SDL_GamepadButton button = SDL_GetGamepadButtonFromString(value.c_str());
      if (button == SDL_GAMEPAD_BUTTON_INVALID) {
        throw std::invalid_argument("Unknown gamepad button: " + value);
      }
      return FromGamepadButton(button);
    }
    if (type_kind == "GamepadAxis") {
      const SDL_GamepadAxis axis = SDL_GetGamepadAxisFromString(value.c_str());
      if (axis == SDL_GAMEPAD_AXIS_INVALID) {
        throw std::invalid_argument("Unknown gamepad axis: " + value);
      }
      return FromGamepadAxis(axis);
    }

    throw std::invalid_argument("Unknown input type: " + type_kind);
  }

  bool operator==(const InputCode& other) const {
    return device == other.device && kind == other.kind && data == other.data;
  }

  bool operator!=(const InputCode& other) const {
    return !(*this == other);
  }
};

} // namespace sb::input
#endif //INPUTCODE_H
