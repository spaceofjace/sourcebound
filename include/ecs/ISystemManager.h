//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef ISYSTEMMANAGER_H
#define ISYSTEMMANAGER_H
#include "Entity.h"
#include "ecs_config.h"

namespace sb::ecs {

class ISystemManager {
public:
  virtual ~ISystemManager() = default;
  ISystemManager() = default;

  ISystemManager(const ISystemManager&) = delete;
  ISystemManager& operator=(const ISystemManager&) = delete;
  ISystemManager(ISystemManager&&) = delete;
  ISystemManager& operator=(ISystemManager&&) = delete;


  virtual void entity_destroyed(Entity entity) = 0;
  virtual void entity_signature_changed(Entity entity, const sb::ecs::Signature& signature) = 0;
};

}  // namespace sb::ecs
#endif //ISYSTEMMANAGER_H
