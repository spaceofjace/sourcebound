//
// Created by shult on 6/20/2025.
//

#ifndef SDLEVENTSOURCE_H
#define SDLEVENTSOURCE_H
#include "IEventSource.h"
#include "SDL3/SDL_events.h"

namespace sb::input {
class SDLEventSource : public IEventSource {
public:
  bool poll_event(SDL_Event& event) override {
    return SDL_PollEvent(&event);
  }
};
} // namespace sb::input

#endif //SDLEVENTSOURCE_H
