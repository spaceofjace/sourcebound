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
#include "../include/data/GameData.h"
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

  // For now, using hardcoded game data; later, I expect this to be managed by a data layer
  const sb::data::LevelData level_data;

  const auto window_height = level_data.arena_height
      + level_data.top_margin
      + level_data.bottom_margin;

  const auto window_width = level_data.arena_width
        + level_data.left_margin
        + level_data.right_margin;

  SDL_Window* window = SDL_CreateWindow("Sourcebound Test", static_cast<int>(window_width),
    static_cast<int>(window_height), 0);

  if (window == nullptr) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, nullptr);
  if (sdl_renderer == nullptr) {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  //I started to move this into initialize, but doing so would "kill" testability
  // Going to remove templated calls as a primary method of using them in a follow-up refactor
  auto renderer = std::make_shared<sb::rendering::SdlRenderer>(sdl_renderer);
  auto render_system = system_mgr->register_system<sb::ecs::RenderSystem>(renderer, component_mgr);

  world->initialize(renderer);

  sb::ecs::Signature renderSig;
  renderSig.reset();

  renderSig.set(component_mgr->get_component_type<Transform>());
  renderSig.set(component_mgr->get_component_type<RenderableSimpleShape>());
  system_mgr->set_signature<sb::ecs::RenderSystem>(renderSig);

  world->unload_level();
  world->load_level(level_data);

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