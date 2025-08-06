#include <chrono>
#include <memory>

#include "../include/data/HardcodedGameDataManager.h"
#include "../include/data/LevelData.h"
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
#include "../include/data/ArenaDimensions.h"
#include "../include/gamestate/StageLifecycleState.h"
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
  auto game_data_manager = std::make_shared<sb::data::HardcodedGameDataManager>();
  game_data_manager->load_config("");
  game_data_manager->set_current_level(1); // no level chain yet

  const auto& level_data = game_data_manager->get_current_level_data();

  auto arena_dimensions = calculate_arena_dimensions(level_data);

  SDL_Window* window = SDL_CreateWindow(level_data.level_name.c_str(), static_cast<int>(arena_dimensions.window_size.x),
    static_cast<int>(arena_dimensions.window_size.y), 0);

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
  auto render_system = std::make_shared<sb::ecs::RenderSystem>(renderer);

  world->initialize(render_system, game_data_manager);
  world->unload_level(); //technically not needed, but want to show the "full flow" here
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

    if (world->get_stage_lifecycle_state() == sb::gamestate::StageLifecycleState::Quit) {
      break;
    }
    if (world->get_stage_lifecycle_state() == sb::gamestate::StageLifecycleState::GameOver) {
      world->unload_level();
      world->load_level(game_data_manager->get_current_level_data());
    }

    world->update(deltaTime);
    renderer->present();
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}