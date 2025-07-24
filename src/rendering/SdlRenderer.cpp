//
// Created by Jace Shultz on 7/9/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/rendering/SdlRenderer.h"

#include <cmath>

namespace sb::rendering {
void SdlRenderer::clear() {
  SetSdlDrawColor(renderer_, Colors::black);
  SDL_RenderClear(renderer_);
}
void SdlRenderer::present() {
  SDL_RenderPresent(renderer_);
}

void SdlRenderer::draw_rect(const math::Vec2& center, const int width, const int height,
    const Color& color, const bool filled) {

  SetSdlDrawColor(renderer_, color);

  // SDL prefers float-based in SDL3, but ints will be simpler for a simple game
  // SDL also uses the corner for rendering position, so we need to convert from center
  const SDL_FRect rect = {
    (center.x - (static_cast<float>(width) * 0.5F)),
    (center.y - (static_cast<float>(height) * 0.5F)),
    static_cast<float>(width),
    static_cast<float>(height)
  };

  filled ? SDL_RenderFillRect(renderer_, &rect)
         : SDL_RenderRect(renderer_, &rect);
}

// NOTE: This is a naive circle rasterizer for debug/simplicity.
// For now, this approach is performant enough for 1–2 small circles per frame.
// Some future optimization considerations:
//
//  - Using a cached texture (filled circle drawn once to texture, then blitted)
//  - Switching to a shader-based or OpenGL pipeline
//  - Drawing with SDL_gfx if ported to SDL3 (not currently official)
//
void SdlRenderer::draw_circle(const math::Vec2& center, const int radius, // NOLINT(*-easily-swappable-parameters)
    const Color& color, const bool filled) {

  SetSdlDrawColor(renderer_, color);

  // SDL prefers float-based in SDL3, but ints will be simpler for a simple game
  const auto f_center_x = static_cast<float>(center.x);
  const auto f_center_y = static_cast<float>(center.y);

  // Loop over a square region centered on (center_x, center_y)
  // This covers the bounding box of the circle.
  for (int dy = -radius; dy <= radius; ++dy) {
    for (int dx = -radius; dx <= radius; ++dx) {

      // Compute squared distance from the center: distance^2 = dx^2 + dy^2
      // This comes from the equation of a circle: (x - cx)^2 + (y - cy)^2 = r^2
      // We’re checking points where:
      //  dx = x - cx
      //  dy = y - cy
      const int dist_sq = (dx * dx) + (dy * dy);
      const int radius_sq = radius * radius;

      if (filled) {
        // For filled circles, draw all points inside or on the circle:
        //     dx^2 + dy^2 <= r^2
        if (dist_sq <= radius_sq) {
          SDL_RenderPoint(renderer_, f_center_x + static_cast<float>(dx),
            f_center_y + static_cast<float>(dy));
        }
      } else {
        // For outlined circles, draw only points near the circle edge.
        // Since we're working with integer pixels, we approximate the edge by accepting points
        // where |dx^2 + dy^2 - r^2| < r
        // This *should* form a ring around the ideal circle perimeter.
        if (std::abs(dist_sq - radius_sq) < radius) {
          SDL_RenderPoint(renderer_, f_center_x + static_cast<float>(dx),
            f_center_y + static_cast<float>(dy));
        }
      }
    }
  }
}

} // namespace sb::rendering
