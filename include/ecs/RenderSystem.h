//
// Created by Jace Shultz on 7/16/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

/**
 * @file RenderSystem.h
 * @ingroup ECS
 * @brief [brief description of file's purpose]
 * 
 * @author Jace Shultz
 * @date 7/16/2025
 */
#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include <memory>
#include "ISystem.h"
#include "../rendering/IRenderer.h"
#include "EntityManager.h"
#include "IComponentManager.h"

namespace sb::ecs {
/**
 * @class RenderSystem
 * @ingroup ECS
 * @brief Responsible for rendering entities with a RenderableSimpleShape and Transform.
 *
 * This system does not call clear() or present(); those are managed externally in the render loop.
 */
class RenderSystem final : public ISystem {
public:
  RenderSystem(std::shared_ptr<rendering::IRenderer> renderer,
               std::shared_ptr<ComponentManager> component_mgr)
    : renderer_(std::move(renderer)),
      component_mgr_(std::move(component_mgr)) {};

  void update(float delta_time) override;

private:
  std::shared_ptr<rendering::IRenderer> renderer_;
  std::shared_ptr<ComponentManager> component_mgr_;
};
} // namespace sb::ecs
#endif //RENDERSYSTEM_H
