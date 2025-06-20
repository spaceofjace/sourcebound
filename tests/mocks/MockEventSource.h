//
// Created by Jace Shultz on 6/20/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//
#ifndef MOCKEVENTSOURCE_H
#define MOCKEVENTSOURCE_H

#include <gmock/gmock.h>
#include "../../../include/input/IEventSource.h"

class MockEventSource : public sb::input::IEventSource {
public:
  MOCK_METHOD(bool, poll_event, (SDL_Event& event), (override));
};

#endif // MOCKEVENTSOURCE_H
