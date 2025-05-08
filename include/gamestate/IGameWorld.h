//
// Created by Jace Shultz on 5/8/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef IGAMEWORLD_H
#define IGAMEWORLD_H

#include "../ecs/Entity.h"

namespace sb::ecs {

class IGameWorld {
public:
  IGameWorld() = default;
  virtual ~IGameWorld() = default;

  IGameWorld(const IGameWorld&) = delete;
  IGameWorld& operator=(const IGameWorld&) = delete;
  IGameWorld(IGameWorld&&) = delete;
  IGameWorld& operator=(IGameWorld&&) = delete;

  virtual Entity create_entity() = 0;
  virtual void destroy_entity(Entity entity) = 0;
  virtual void update(float delta_time) = 0;
  virtual void process_events() = 0;
  [[nodiscard]] virtual bool is_alive(Entity entity) const = 0;
};

} // namespace sb::ecs
#endif //IGAMEWORLD_H
