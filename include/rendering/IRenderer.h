//
// Created by Jace Shultz on 7/9/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @defgroup Rendering Rendering Layer
 * @brief Core module defining rendering interfaces and draw operations.
 *
 * This group contains interfaces and tools related to the rendering layer,
 * including frame lifecycle methods and basic drawing primitives. It enables
 * separation of rendering logic from backend implementation.
 */

/**
 * @file IRenderer.h
 * @ingroup Rendering
 9* @brief Interface definition for abstract rendering operations.
 * 
 * @author Jace Shultz
 * @date 7/9/2025
 */

#ifndef IRENDERER_H
#define IRENDERER_H
#include "Color.h"

namespace sb::rendering {

/**
 * @interface IRenderer
 * @ingroup Rendering
 * @brief Abstract interface for the rendering layer.
 *
 * Provides a minimal drawing API to support testable rendering logic without binding to a specific
 * graphics backend.  Theoretically can be DirectX,
 */
class IRenderer {
public:

  IRenderer() = default;
  virtual ~IRenderer() = default;

  IRenderer(const IRenderer&) = delete;
  IRenderer& operator=(const IRenderer&) = delete;
  IRenderer(IRenderer&&) = delete;
  IRenderer& operator=(IRenderer&&) = delete;

/**
 * @brief Clears the current rendering surface.
 */
  virtual void clear() = 0;

/**
 * @brief Presents the rendered frame to the display.
 */
  virtual void present() = 0;

/**
 * @brief Draws a filled rectangle.
 *
 * @param pos_x The X coordinate of the rectangle's top-left corner.
 * @param pos_y The Y coordinate of the rectangle's top-left corner.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color Defines color channels (RGBA) to render.
 * @param filled filled if true, otherwise a 1 pixel outline
 */
  virtual void draw_rect(int pos_x, int pos_y, int width, int height, const Color& color,
    bool filled) = 0;

/**
 * @brief Draws a filled circle.
 *
 * @param center_x X coordinate of the circle center.
 * @param center_y Y coordinate of the circle center.
 * @param radius Radius of the circle.
 * @param color Defines color channels (RGBA) to render.
 * @param filled filled if true, otherwise a 1 pixel outline
 */
  virtual void draw_circle(int center_x, int center_y, int radius, const Color& color,
    bool filled) = 0;
};

}  // namespace sb::rendering
#endif //IRENDERER_H
