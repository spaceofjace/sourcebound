//
// Created by Jace Shultz on 6/19/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/input/KeyboardHandler.h"
#include "SDL3/SDL_events.h"
#include "../../include/input/InputCode.h"

namespace sb::input{

void KeyboardHandler::poll_inputs() {
  SDL_Event event;
  bool quit = false;
  while (!quit && this->event_source().poll_event(event)) {
    if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
      const SDL_KeyboardEvent& key_event = event.key;

      // Ignore key repeat events?
      if (event.type == SDL_EVENT_KEY_DOWN && key_event.repeat != 0) {
        continue;
      }

      const SDL_Scancode scancode = key_event.scancode;
      const bool is_pressed = (event.type == SDL_EVENT_KEY_DOWN);

      InputCode code = InputCode::FromKeyboard(scancode);
      this->normalizer().normalize_input(code, is_pressed ? 1.0F : 0.0F);
    }
    else if (event.type == SDL_EVENT_QUIT) {
      quit = true;
      //Do I need a QuitCommand?
    }
  }
}

} // namespace sb::input