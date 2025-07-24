//
// Created by Jace Shultz on 7/9/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file SdlRenderer.h
 * @ingroup Rendering
 * @brief SDL implementation of the IRenderer interface.
 * 
 * @author Jace Shultz
 * @date 7/9/2025
 */
#ifndef SDLRENDERER_H
#define SDLRENDERER_H
#include <SDL3/SDL_render.h>
#include "IRenderer.h"


namespace sb::rendering {
class SdlRenderer final : public IRenderer {
 public:
  explicit SdlRenderer(SDL_Renderer* sdl_renderer) : renderer_(sdl_renderer) {}
  ~SdlRenderer() override = default;

  SdlRenderer(const SdlRenderer&) = delete;
  SdlRenderer& operator=(const SdlRenderer&) = delete;
  SdlRenderer(SdlRenderer&&) = delete;
  SdlRenderer& operator=(SdlRenderer&&) = delete;

/**
 * @brief Clears the current rendering target using the active draw color.
 *
 * Internally calls SDL_RenderClear after setting the appropriate color. This should be called at
 * the beginning of each frame.
 */
  void clear() override;

/**
 * @brief Presents the rendered content to the display.
 *
 * Finalizes the frame by calling SDL_RenderPresent. This should be called at the end of each
 * frame after all draw calls.
 */
  void present() override;

/**
 * @brief Draws a rectangle to the rendering target.
 *
 * Uses SDL_RenderFillRect or SDL_RenderDrawRect depending on the filled flag.
 * SDL uses top corner for render position, so a conversion from center -> top corner is needed.
 *
 * @param center The center point of the rectangle.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Defines color channels (RGBA) to render.
 * @param filled If true, fills the rectangle; otherwise, draws a 1-pixel outline.
 */
  void draw_rect(const math::Vec2& center, int width, int height, const Color& color, bool filled) override;

/**
 * @brief Draws a circle to the rendering target.
 *
 * This method manually rasterizes the circle, since SDL has no built-in circle API.
 *
 * @param center The center point of the circle.
 * @param radius Radius of the circle.
 * @param color Defines color channels (RGBA) to render.
 * @param filled If true, fills the circle; otherwise, draws a 1-pixel outline.
 */
  void draw_circle(const math::Vec2& center, int radius, const Color& color, bool filled) override;

 private:
  // SDL manages shutdown through SDL_DestroyRenderer, so using a standard pointer here as cleanup
  // will happen external to this class.
  SDL_Renderer* renderer_;

  static void SetSdlDrawColor(SDL_Renderer* renderer, const Color& color) {
    SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);
  }
};
} // namespace sb::rendering

#endif //SDLRENDERER_H
