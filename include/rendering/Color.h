//
// Created by Jace Shultz on 7/9/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file Color.h
 * @ingroup Rendering
 * @brief Defines the Color struct used for RGBA color representation and a set of standard colors.
 *
 * Provides a simple, constexpr-compatible struct for expressing colors with red, green, blue, and alpha channels.
 * Includes a namespace of common predefined color constants.
 *
 * @author Jace Shultz
 * @date 7/9/2025
 */
#ifndef COLOR_H
#define COLOR_H
namespace sb::rendering {

/**
 * @struct Color
 * @ingroup Rendering
 * @brief Represents an RGBA color.
 *
 * Encapsulates a red, green, blue, and alpha channel, each in the range 0–255.
 * Used throughout the rendering system to specify draw colors.
 */
struct Color {
  static constexpr int kMinChannelValue = 0;
  static constexpr int kMaxChannelValue = 255;

  int red;
  int green;
  int blue;
  int alpha = kMaxChannelValue;

  // suppressing warning as these follow the standard RGBA pattern
  constexpr Color(const int red_channel, const int green_channel, const int blue_channel, // NOLINT(*-easily-swappable-parameters)
                  const int alpha_channel = kMaxChannelValue)
      : red(red_channel), green(green_channel), blue(blue_channel), alpha(alpha_channel) {}

  bool operator==(const Color& rhs) const {
    return red == rhs.red &&
           green == rhs.green &&
           blue == rhs.blue &&
           alpha == rhs.alpha;
  }
};

namespace Colors {
  // Main solids
  inline constexpr Color white{255, 255, 255};
  inline constexpr Color black{0, 0, 0};
  inline constexpr Color red{255, 0, 0};
  inline constexpr Color green{0, 255, 0};
  inline constexpr Color blue{0, 0, 255};
  inline constexpr Color transparent{0, 0, 0, 0};

  // Extended colors
  inline constexpr Color orange{255, 165, 0};
  inline constexpr Color yellow{255, 255, 0};
  inline constexpr Color brown{139, 69, 19};
  inline constexpr Color cyan{0, 255, 255};
  inline constexpr Color magenta{255, 0, 255};
  inline constexpr Color purple{128, 0, 128};

  // Grayscale colors
  inline constexpr Color gray{128, 128, 128};
  inline constexpr Color light_gray{192, 192, 192};
  inline constexpr Color dark_gray{64, 64, 64};
} // namespace Colors

}  // namespace sb::rendering
#endif //COLOR_H
