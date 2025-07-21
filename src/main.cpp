#include <chrono>
#include <memory>

#include "../include/ecs/EntityManager.h"
#include "../include/ecs/IComponentManager.h"
#include "../include/ecs/RenderSystem.h"
#include "../include/ecs/SystemManager.h"
#include "../include/ecs/components/Components.h"
#include "../include/gamestate/CommandQueue.h"
#include "../include/gamestate/GameWorld.h"
#include "../include/input/HardcodedBindingMap.h"
#include "../include/input/InputNormalizer.h"
#include "../include/input/KeyboardHandler.h"
#include "../include/input/SdlEventSource.h"
#include "../include/rendering/SdlRenderer.h"
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
  auto event_source = std::make_shared<sb::input::SdlEventSource>();
  sb::input::KeyboardHandler input(normalizer, event_source);

  auto world = std::make_shared<sb::gamestate::GameWorld>(
      entity_mgr, component_mgr, system_mgr, cmd_queue
  );

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow("Sourcebound Test", 800, 600, 0);

  if (!window) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, nullptr);
  if (!sdl_renderer) {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  world->register_component<Velocity>();
  world->register_component<Paddle>();
  world->register_component<Ball>();
  world->register_component<Transform>();
  world->register_component<RenderableSimpleShape>();

  auto renderer = std::make_shared<sb::rendering::SdlRenderer>(sdl_renderer);
  auto render_system = system_mgr->register_system<sb::ecs::RenderSystem>(renderer, component_mgr);

  sb::ecs::Signature renderSig;
  renderSig.reset();

  renderSig.set(component_mgr->get_component_type<Transform>());
  renderSig.set(component_mgr->get_component_type<RenderableSimpleShape>());
  system_mgr->set_signature<sb::ecs::RenderSystem>(renderSig);

  auto paddle = world->create_entity();
  world->add_component(paddle, Paddle{});
  world->add_component(paddle, Transform{{100, 150}, {}, {100, 10}, {}});
  world->add_component(paddle, RenderableSimpleShape{sb::rendering::Colors::blue, SimpleShapeType::Rectangle, true});
  world->add_component(paddle, Velocity{0, 0});

  auto ball = world->create_entity();
  world->add_component(ball, Ball{});
  world->add_component(ball, Transform{{150, 100}, {}, {20, 20}, {}});
  world->add_component(ball, RenderableSimpleShape{sb::rendering::Colors::cyan, SimpleShapeType::Circle, true});
  world->add_component(ball, Velocity{0, 0});

  TimePoint lastFrameTime = Clock::now();

  while (true) {
    TimePoint currentFrameTime = Clock::now();
    std::chrono::duration<float> elapsed = currentFrameTime - lastFrameTime;
    const float deltaTime = elapsed.count();
    lastFrameTime = currentFrameTime;

    // Main per-frame execution
    input.poll_inputs();
    renderer->clear();
    world->step(deltaTime);

    if (world->should_exit()) {
      break;
    }

    world->update(deltaTime);
    renderer->present();
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}