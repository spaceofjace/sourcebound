//
// Created by Jace Shultz on 5/8/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#ifndef MOCKSYSTEM_H
#define MOCKSYSTEM_H

#include "../../include/ecs/ISystem.h"

using namespace sb::ecs;

struct MockSystem : ISystem {
  void update(float /*delta_time*/) override {}
};

#endif //MOCKSYSTEM_H
