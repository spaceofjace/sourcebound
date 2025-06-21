//
// Created by shult on 6/20/2025.
//

#ifndef IEVENTSOURCE_H
#define IEVENTSOURCE_H
#include "SDL3/SDL_events.h"
namespace sb::input {
class IEventSource {
public:
  virtual ~IEventSource() = default;
  virtual bool poll_event(SDL_Event& event) = 0;
};
}// namespace sb::input
#endif //IEVENTSOURCE_H
