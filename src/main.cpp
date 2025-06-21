#include <chrono>
#include <memory>

#include "../include/ecs/EntityManager.h"
#include "../include/ecs/IComponentManager.h"
#include "../include/ecs/SystemManager.h"
#include "../include/ecs/components/Components.h"
#include "../include/gamestate/CommandQueue.h"
#include "../include/gamestate/GameWorld.h"
#include "../include/input/HardcodedBindingMap.h"
#include "../include/input/InputNormalizer.h"
#include "../include/input/KeyboardHandler.h"
#include "../include/input/SDLEventSource.h"
#include "SDL3/SDL.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

int main() {
  auto entity_mgr = std::make_shared<sb::ecs::EntityManager>(true,500);
  auto component_mgr = std::make_shared<sb::ecs::ComponentManager>();
  auto system_mgr = std::make_shared<sb::ecs::SystemManager>();
  auto cmd_queue = std::make_shared<sb::gamestate::CommandQueue>();

  auto bindings = std::make_shared<sb::input::HardcodedBindingMap>();
  auto normalizer = std::make_shared<sb::input::InputNormalizer>(bindings, cmd_queue);
  auto event_source = std::make_shared<sb::input::SDLEventSource>();
  sb::input::KeyboardHandler input(normalizer, event_source);

  auto world = std::make_shared<sb::gamestate::GameWorld>(
      entity_mgr, component_mgr, system_mgr, cmd_queue
  );

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow("Sourcebound Test", 800, 600, 0);

  if (!window) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  TimePoint lastFrameTime = Clock::now();
  bool isRunning = true;

  world->register_component<Velocity>();
  world->register_component<Paddle>();

  auto paddle = world->create_entity();
  world->add_component(paddle, Paddle{});
  world->add_component(paddle, Velocity{0, 0});

  while (isRunning) {
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {
      if (sdl_event.type == SDL_EVENT_QUIT) {
        isRunning = false;
      }
    }

    TimePoint currentFrameTime = Clock::now();
    std::chrono::duration<float> elapsed = currentFrameTime - lastFrameTime;
    float deltaTime = elapsed.count();
    lastFrameTime = currentFrameTime;

    input.poll_inputs();
    // Main per-frame execution
    world->step(deltaTime);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}