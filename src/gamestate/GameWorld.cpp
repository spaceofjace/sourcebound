//
// Created by Jace Shultz on 7/24/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/gamestate/GameWorld.h"

#include "../../include/ecs/systems/PhysicsSystem.h"
#include "../../include/ecs/RenderSystem.h"
#include "../../include/ecs/components/Components.h"

using sb::ecs::Entity;
using sb::ecs::Signature;
using sb::ecs::PhysicsSystem;
using sb::ecs::RenderSystem;

void sb::gamestate::GameWorld::initialize(std::shared_ptr<ecs::ISystem> render_system,
    std::shared_ptr<data::IGameDataManager> game_data_manager) {

  //Register any game components used for this game
  register_components();
  register_systems(std::move(render_system), game_data_manager);
}

Entity sb::gamestate::GameWorld::create_entity() {
  return entity_manager_->create_entity();
}

void sb::gamestate::GameWorld::destroy_entity(const Entity entity) {
  entity_manager_->destroy_entity(entity);
}

void sb::gamestate::GameWorld::step(const float delta_time) {
  cmd_queue_->process(shared_from_this());
  cmd_queue_->clear();
  update(delta_time);
}

void sb::gamestate::GameWorld::update(const float delta_time) {
  //Will have more here as more comes online
  system_manager_->update(delta_time);
}

void sb::gamestate::GameWorld::process_events() {
  //TODO Requires event manager system
}

[[nodiscard]] bool sb::gamestate::GameWorld::is_alive(const Entity entity) const {
  return entity_manager_->is_alive(entity);
}

[[nodiscard]] bool sb::gamestate::GameWorld::should_exit() const {
  return should_exit_;
}

void sb::gamestate::GameWorld::request_exit() {
  should_exit_ = true;
}

void sb::gamestate::GameWorld::load_level(const data::LevelData& level_data) {
  const float window_width =
      level_data.arena_width + level_data.left_margin + level_data.right_margin;
  const float window_height =
      level_data.arena_height + level_data.top_margin + level_data.bottom_margin;

  // Left Wall
  const auto left_wall = create_entity();
  add_component(left_wall, Wall{});
  const float left_wall_x = level_data.left_margin + (level_data.wall_thickness / 2.0F);
  const float vertical_wall_center_y = window_height / 2.0F;
  add_component(left_wall, Transform{{left_wall_x, vertical_wall_center_y},
                                     {level_data.wall_thickness, level_data.arena_height},
                                     {0}});
  add_component(left_wall,
                RenderableSimpleShape{rendering::Colors::red, SimpleShapeType::Rectangle, true});
  add_component(left_wall, BoxCollider{level_data.wall_thickness, level_data.arena_height, 0, 0,
                                       CollisionBehavior::Clamp | CollisionBehavior::Bounce});

  // Right Wall
  const auto right_wall = create_entity();
  add_component(right_wall, Wall{});
  const float right_wall_x =
      window_width - level_data.right_margin - (level_data.wall_thickness / 2.0F);
  add_component(right_wall, Transform{{right_wall_x, vertical_wall_center_y},
                                      {level_data.wall_thickness, level_data.arena_height},
                                      {0}});
  add_component(right_wall,
                RenderableSimpleShape{rendering::Colors::red, SimpleShapeType::Rectangle, true});
  add_component(right_wall, BoxCollider{level_data.wall_thickness, level_data.arena_height, 0, 0,
                                        CollisionBehavior::Clamp | CollisionBehavior::Bounce});

  // Top Wall
  const auto top_wall = create_entity();
  add_component(top_wall, Wall{});
  const float top_wall_y = level_data.top_margin + (level_data.wall_thickness / 2.0F);
  const float horizontal_wall_center_x = window_width / 2.0f;
  add_component(top_wall, Transform{{horizontal_wall_center_x, top_wall_y},
                                    {level_data.arena_width, level_data.wall_thickness},
                                    {0}});
  add_component(top_wall,
                RenderableSimpleShape{rendering::Colors::red, SimpleShapeType::Rectangle, true});
  add_component(top_wall, BoxCollider{level_data.arena_width, level_data.wall_thickness, 0, 0,
                                      CollisionBehavior::Clamp | CollisionBehavior::Bounce});

  // Bottom Wall (out of bounds)
  const auto bottom_wall = create_entity();
  add_component(bottom_wall, Wall{});
  const float bottom_wall_y =
      window_height - level_data.bottom_margin - (level_data.wall_thickness / 2.0F);
  add_component(bottom_wall, Transform{{horizontal_wall_center_x, bottom_wall_y},
                                       {level_data.arena_width - (level_data.wall_thickness * 2),
                                        level_data.wall_thickness},
                                       {0}});
  add_component(bottom_wall, RenderableSimpleShape{rendering::Colors::yellow,
                                                   SimpleShapeType::Rectangle, false});
  add_component(bottom_wall, BoxCollider{level_data.arena_width, level_data.wall_thickness, 0, 0,
                                         CollisionBehavior::Destroy | CollisionBehavior::Trigger});

  // Paddle
  const float paddle_x =
      level_data.left_margin + (level_data.arena_width / 2.0F) + level_data.paddle_offset.x;
  const float paddle_y = window_height - level_data.bottom_margin - level_data.wall_thickness -
                         (level_data.paddle_height / 2.0F) + level_data.paddle_offset.y;

  auto paddle = create_entity();
  add_component(paddle, Paddle{});
  add_component(paddle, Transform{{paddle_x, paddle_y}, {level_data.paddle_width,
    level_data.paddle_height}, {0}});
  add_component(paddle, RenderableSimpleShape{rendering::Colors::blue,
    SimpleShapeType::Rectangle, true});
  add_component(paddle, Velocity{0, 0});
  add_component(paddle, Direction{0, 0});

  // Ball
  const float ball_x = paddle_x + level_data.ball_offset.x;
  const float ball_y = paddle_y - (level_data.paddle_height / 2.0f) - level_data.ball_radius +
                       level_data.ball_offset.y;
  const float ball_diameter = level_data.ball_radius * 2.0f;

  auto ball = create_entity();
  add_component(ball, Ball{});
  add_component(ball, StuckToPaddle{});
  add_component(ball, Transform{{ball_x, ball_y}, {ball_diameter,
    ball_diameter}, {0}});
  add_component(ball, RenderableSimpleShape{rendering::Colors::cyan,
    SimpleShapeType::Circle, true});
  add_component(ball, Velocity{0, 0});
  add_component(ball, Direction{0, 0});
}

void sb::gamestate::GameWorld::unload_level() {
  entity_manager_->clear_all();
}

[[nodiscard]] std::vector<Entity> sb::gamestate::GameWorld::get_entities_with_signature(
    const Signature& target_signature) const {
  return entity_manager_->get_entities_with_signature(target_signature);
}

void sb::gamestate::GameWorld::register_components() const {
  // may add "auto-registration" via static initializer later
  register_component<Velocity>();
  register_component<Paddle>();
  register_component<Ball>();
  register_component<Transform>();
  register_component<RenderableSimpleShape>();
  register_component<Wall>();
  register_component<BoxCollider>();
  register_component<StuckToPaddle>();
  register_component<Direction>();
}

// ReSharper disable once CppMemberFunctionMayBeConst
// actually mutates through shared pointers
void sb::gamestate::GameWorld::register_systems(std::shared_ptr<ecs::ISystem> render_system,
    std::shared_ptr<data::IGameDataManager> game_data_manager) {
  Signature renderSig;
  renderSig.reset();

  system_manager_->register_system(typeid(RenderSystem), std::move(render_system));
  renderSig.set(component_manager_->get_component_type<Transform>());
  renderSig.set(component_manager_->get_component_type<RenderableSimpleShape>());
  system_manager_->set_signature(typeid(RenderSystem), renderSig);

  //Will soon try to refactor to allow bindings so this can be injectable
  Signature physicsSig;
  physicsSig.reset();
  physicsSig.set(component_manager_->get_component_type<Transform>());
  physicsSig.set(component_manager_->get_component_type<Velocity>());
  physicsSig.set(component_manager_->get_component_type<Direction>());

  auto physics_system = std::make_shared<PhysicsSystem>(game_data_manager, component_manager_);
  system_manager_->register_system(typeid(PhysicsSystem), physics_system);
  system_manager_->set_signature(typeid(PhysicsSystem), physicsSig);
}