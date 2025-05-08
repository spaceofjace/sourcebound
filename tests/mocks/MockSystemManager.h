//
// Created by Jace Shultz on 5/8/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef MOCKSYSTEMMANAGER_H
#define MOCKSYSTEMMANAGER_H

#include "../../include/ecs/ISystemManager.h"

using namespace sb::ecs;

struct MockSystemManager : ISystemManager {
  void update(float /*delta_time*/) override {}
  void entity_destroyed(Entity entity) override{}
  void entity_signature_changed(Entity entity, const Signature& signature) override{}
};
#endif //MOCKSYSTEMMANAGER_H
