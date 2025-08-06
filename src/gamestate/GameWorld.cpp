//
// Created by Jace Shultz on 7/24/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../../include/gamestate/GameWorld.h"


#include "../../include/data/ArenaDimensions.h"
#include "../../include/ecs/RenderSystem.h"
#include "../../include/ecs/components/Components.h"
#include "../../include/ecs/systems/CollisionSystem.h"
#include "../../include/ecs/systems/FollowSystem.h"
#include "../../include/ecs/systems/PhysicsSystem.h"
#include "../../include/ecs/systems/TriggerSystem.h"
#include "../../include/ecs/systems/HitSystem.h"
#include "../../include/ecs/systems/DestructionSystem.h"
#include "../../include/ecs/systems/LivesSystem.h"


using sb::ecs::CollisionSystem;
using sb::ecs::Signature;
using sb::ecs::PhysicsSystem;
using sb::ecs::RenderSystem;
using sb::ecs::CollisionSystem;
using sb::ecs::FollowSystem;
using sb::ecs::Entity;

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
  component_manager_->entity_destroyed(entity);
  system_manager_->entity_destroyed(entity);
}

void sb::gamestate::GameWorld::step(const float delta_time) {
  cmd_queue_->process(shared_from_this());
  cmd_queue_->clear();
  update(delta_time);
}

void sb::gamestate::GameWorld::update(const float delta_time) {
  //Will have more here as more comes online
  system_manager_->update_all(delta_time, *this);
}

void sb::gamestate::GameWorld::process_events() {
  //TODO Requires event manager system
}

[[nodiscard]] bool sb::gamestate::GameWorld::is_alive(const Entity entity) const {
  return entity_manager_->is_alive(entity);
}

void sb::gamestate::GameWorld::load_level(const data::LevelData& level_data) {

  auto arena_dimensions = calculate_arena_dimensions(level_data);

  // Using 2 * thickness to creat corner overlaps
  const float vertical_wall_height = arena_dimensions.arena_size.y + (level_data.wall_thickness * 2);
  const float horizontal_wall_width = arena_dimensions.arena_size.x + (level_data.wall_thickness * 2);

  // Left Wall
  const auto left_wall = create_entity();
  add_component(left_wall, ecs::Wall{});
  const float left_wall_x = level_data.outer_left_margin + (level_data.wall_thickness / 2.0F);
  const float vertical_wall_center_y = arena_dimensions.window_size.y / 2.0F;
  add_component(left_wall, ecs::Transform{{left_wall_x, vertical_wall_center_y},
    {level_data.wall_thickness, vertical_wall_height}, {0}});
  add_component(left_wall, ecs::RenderableSimpleShape{rendering::Colors::red, sb::ecs::SimpleShapeType::Rectangle, true});
  add_component(left_wall, ecs::BoxCollider{level_data.wall_thickness, vertical_wall_height, 0, 0,
    ecs::CollisionBehavior::Clamp | ecs::CollisionBehavior::Bounce});
  add_component(left_wall, ecs::Indestructible{});

  // Right Wall
  const auto right_wall = create_entity();
  add_component(right_wall, ecs::Wall{});
  const float right_wall_x = arena_dimensions.window_size.x - level_data.outer_right_margin -
    (level_data.wall_thickness / 2.0F);
  add_component(right_wall, ecs::Transform{{right_wall_x, vertical_wall_center_y},
    {level_data.wall_thickness, vertical_wall_height}, {0}});
  add_component(right_wall, ecs::RenderableSimpleShape{rendering::Colors::red,
    ecs::SimpleShapeType::Rectangle, true});
  add_component(right_wall, ecs::BoxCollider{level_data.wall_thickness, vertical_wall_height,
    0, 0, ecs::CollisionBehavior::Clamp | ecs::CollisionBehavior::Bounce});
  add_component(right_wall, ecs::Indestructible{});

  // Top Wall
  const auto top_wall = create_entity();
  add_component(top_wall, ecs::Wall{});
  const float top_wall_y = level_data.outer_top_margin + (level_data.wall_thickness / 2.0F);
  const float horizontal_wall_center_x = arena_dimensions.window_size.x / 2.0F;
  add_component(top_wall, ecs::Transform{{horizontal_wall_center_x, top_wall_y},
    {horizontal_wall_width, level_data.wall_thickness},{0}});
  add_component(top_wall, ecs::RenderableSimpleShape{rendering::Colors::red,
    ecs::SimpleShapeType::Rectangle, true});
  add_component(top_wall, ecs::BoxCollider{horizontal_wall_width, level_data.wall_thickness, 0, 0,
    ecs::CollisionBehavior::Clamp | ecs::CollisionBehavior::Bounce});
  add_component(top_wall, ecs::Indestructible{});

  // Bottom Wall (out of bounds)
  const auto bottom_wall = create_entity();
  add_component(bottom_wall, ecs::Wall{});
  const float bottom_wall_y = arena_dimensions.window_size.y - level_data.outer_bottom_margin - (level_data.wall_thickness / 2.0F);
  add_component(bottom_wall, ecs::Transform{{horizontal_wall_center_x, bottom_wall_y},
    {horizontal_wall_width - (level_data.wall_thickness * 2),
      level_data.wall_thickness}, {0}});
  add_component(bottom_wall, ecs::RenderableSimpleShape{rendering::Colors::yellow,
    ecs::SimpleShapeType::Rectangle, false});
  add_component(bottom_wall, ecs::BoxCollider{horizontal_wall_width - (level_data.wall_thickness * 2), level_data.wall_thickness, 0, 0,
    ecs::CollisionBehavior::Trigger});
  add_component(bottom_wall, ecs::Triggerable { { ecs::TriggerType::Destroy }});

  // Paddle
  const float paddle_x =
      level_data.outer_left_margin + (arena_dimensions.arena_size.x / 2.0F) + level_data.paddle_offset.x;

  const float paddle_y = bottom_wall_y
                     - (level_data.wall_thickness / 2.0F)
                     - (level_data.paddle_height / 2.0F)
                     + level_data.paddle_offset.y;

  auto paddle = create_entity();
  add_component(paddle, ecs::Paddle{});
  add_component(paddle, ecs::Transform{{paddle_x, paddle_y}, {level_data.paddle_width,
    level_data.paddle_height}, {0}});
  add_component(paddle, ecs::RenderableSimpleShape{rendering::Colors::blue,
    ecs::SimpleShapeType::Rectangle, true});
  add_component(paddle, ecs::Velocity{0, 0});
  add_component(paddle, ecs::Direction{0, 0});
  add_component(paddle, ecs::BoxCollider{level_data.paddle_width, level_data.paddle_height, 0, 0,
    ecs::CollisionBehavior::Bounce});
  add_component(paddle, ecs::PositionBasedBounce{});
  add_component(paddle, ecs::Indestructible{});

  log::Logger::info("Paddle X: " + std::to_string(paddle_x) + " Y: " + std::to_string(paddle_y));
  log::Logger::info("Paddle Width: " + std::to_string(level_data.paddle_width) + " Height: " + std::to_string(level_data.paddle_height));

  // Ball
  const float ball_x = paddle_x + level_data.ball_start_offset.x;
  const float ball_y = paddle_y - (level_data.paddle_height / 2.0F) - level_data.ball_radius +
                       level_data.ball_start_offset.y;
  const float ball_diameter = level_data.ball_radius * 2.0F;

  auto ball = create_entity();
  add_component(ball, ecs::Ball{});
  add_component(ball, ecs::PositionFollower{
    paddle,
    sb::math::Vec2{0.0F,
      -level_data.ball_radius - (level_data.paddle_height / 2.0F) + level_data.ball_start_offset.y
    }
  });
  add_component(ball, ecs::Transform{{ball_x, ball_y}, {ball_diameter,
    ball_diameter}, {0}});
  add_component(ball, ecs::RenderableSimpleShape{rendering::Colors::cyan,
    ecs::SimpleShapeType::Circle, true});
  add_component(ball, ecs::Velocity{0, 0});
  add_component(ball, ecs::Direction{0, 0});
  add_component(ball, ecs::CircleCollider{level_data.ball_radius, 0, 0});

  // Bricks
  const auto& layout = level_data.brick_layout;
  const auto& grid = level_data.bricks;

  for (int row = 0; row < static_cast<int>(grid.size()); ++row) {
    const auto& brick_row = grid[row];
    for (int col = 0; col < static_cast<int>(brick_row.size()); ++col) {
      const auto& config_opt = brick_row[col];
      if (!config_opt.has_value()) {
        continue; // skip empty cells
      }

      const float start_x = level_data.outer_left_margin + level_data.wall_thickness +
                            level_data.inner_left_margin + (layout.brick_size.x / 2.0F);
      const float start_y = level_data.outer_top_margin + level_data.wall_thickness +
                            level_data.inner_top_margin + (layout.brick_size.y / 2.0F);

      math::Vec2 brick_pos{
          start_x + (static_cast<float>(col) * (layout.brick_size.x + layout.brick_margin.x)),
          start_y + (static_cast<float>(row) * (layout.brick_size.y + layout.brick_margin.y))};

      const sb::data::BrickConfig& config = *config_opt;
      const Entity brick = create_entity();

      add_component(brick, ecs::Brick{});
      add_component(brick, ecs::Transform{ecs::Position{brick_pos},
                                          ecs::Size{layout.brick_size.x, layout.brick_size.y},
                                          {}});

      if (config.visual) {
        add_component(brick, *config.visual);
      }
      if (config.collider) {
        add_component(brick, *config.collider);
      }
      if (config.hits) {
        add_component(brick, *config.hits);
      }
      if (config.triggers) {
        add_component(brick, *config.triggers);
      }
    }
  }

  const auto player_session = create_entity();
  add_component(player_session, ecs::PlayerSessionState{0, 1});

  stage_lifecycle_state_ = StageLifecycleState::Active;
}

void sb::gamestate::GameWorld::unload_level() {
  entity_manager_->clear_all();
  component_manager_->clear_all();
  system_manager_->clear_all();
  cmd_queue_->clear();
  stage_lifecycle_state_ = StageLifecycleState::NotStarted;
}
void sb::gamestate::GameWorld::set_stage_lifecycle_state(const StageLifecycleState new_state) {
  this->stage_lifecycle_state_ = new_state;
}
sb::gamestate::StageLifecycleState sb::gamestate::GameWorld::get_stage_lifecycle_state() {
  return stage_lifecycle_state_;
}

[[nodiscard]] std::vector<Entity> sb::gamestate::GameWorld::get_entities_with_signature(
    const Signature& target_signature) const {
  return entity_manager_->get_entities_with_signature(target_signature);
}

void sb::gamestate::GameWorld::register_components() const {
  // may add "auto-registration" via static initializer later
  register_component<ecs::Velocity>();
  register_component<ecs::Paddle>();
  register_component<ecs::Ball>();
  register_component<ecs::Transform>();
  register_component<ecs::RenderableSimpleShape>();
  register_component<ecs::Wall>();
  register_component<ecs::BoxCollider>();
  register_component<ecs::CircleCollider>();
  register_component<ecs::Direction>();
  register_component<ecs::PositionFollower>();
  register_component<ecs::CollisionBehavior>();
  register_component<ecs::PositionBasedBounce>();
  register_component<ecs::HitsRequired>();
  register_component<ecs::Brick>();
  register_component<ecs::Triggerable>();
  register_component<ecs::WasTriggered>();
  register_component<ecs::PendingHit>();
  register_component<ecs::PendingDestroy>();
  register_component<ecs::Indestructible>();
  register_component<ecs::PlayerSessionState>();
}

// ReSharper disable once CppMemberFunctionMayBeConst
// actually mutates through shared pointers
void sb::gamestate::GameWorld::register_systems(std::shared_ptr<ecs::ISystem> render_system,
    std::shared_ptr<data::IGameDataManager> game_data_manager) {
  Signature render_sig;
  render_sig.reset();

  system_manager_->register_system(typeid(RenderSystem), std::move(render_system));
  render_sig.set(component_manager_->get_component_type<ecs::Transform>());
  render_sig.set(component_manager_->get_component_type<ecs::RenderableSimpleShape>());
  system_manager_->set_signature(typeid(RenderSystem), render_sig);

  //Will soon try to refactor to allow bindings so this can be injectable
  Signature physics_sig;
  physics_sig.reset();
  physics_sig.set(component_manager_->get_component_type<ecs::Transform>());
  physics_sig.set(component_manager_->get_component_type<ecs::Velocity>());
  physics_sig.set(component_manager_->get_component_type<ecs::Direction>());

  auto physics_system = std::make_shared<PhysicsSystem>(game_data_manager);
  system_manager_->register_system(typeid(PhysicsSystem), physics_system);
  system_manager_->set_signature(typeid(PhysicsSystem), physics_sig);

  Signature collision_sig;
  collision_sig.set(component_manager_->get_component_type<ecs::Transform>());

  auto collision_system = std::make_shared<CollisionSystem>(game_data_manager);
  system_manager_->register_system(typeid(CollisionSystem), collision_system);
  system_manager_->set_signature(typeid(CollisionSystem), collision_sig);

  Signature follow_sig;
  follow_sig.set(component_manager_->get_component_type<ecs::Transform>());
  follow_sig.set(component_manager_->get_component_type<ecs::PositionFollower>());

  auto follow_system = std::make_shared<FollowSystem>(game_data_manager);
  system_manager_->register_system(typeid(FollowSystem), follow_system);
  system_manager_->set_signature(typeid(FollowSystem), follow_sig);

  Signature trigger_sig;
  trigger_sig.set(component_manager_->get_component_type<ecs::WasTriggered>());
  trigger_sig.set(component_manager_->get_component_type<ecs::Triggerable>());

  auto trigger_system = std::make_shared<ecs::TriggerSystem>(game_data_manager);
  system_manager_->register_system(typeid(ecs::TriggerSystem), trigger_system);
  system_manager_->set_signature(typeid(ecs::TriggerSystem), trigger_sig);

  Signature hit_sig;
  hit_sig.set(component_manager_->get_component_type<ecs::PendingHit>());
  hit_sig.set(component_manager_->get_component_type<ecs::HitsRequired>());

  auto hit_system = std::make_shared<ecs::HitSystem>(game_data_manager);
  system_manager_->register_system(typeid(ecs::HitSystem), hit_system);
  system_manager_->set_signature(typeid(ecs::HitSystem), hit_sig);

  Signature destruction_sig;
  destruction_sig.set(component_manager_->get_component_type<ecs::PendingDestroy>());

  auto destruction_system = std::make_shared<ecs::DestructionSystem>(game_data_manager);
  system_manager_->register_system(typeid(ecs::DestructionSystem), destruction_system);
  system_manager_->set_signature(typeid(ecs::DestructionSystem), destruction_sig);

  Signature lives_sig;
  lives_sig.set(component_manager_->get_component_type<ecs::Ball>());

  auto lives_system = std::make_shared<ecs::LivesSystem>(game_data_manager);
  system_manager_->register_system(typeid(ecs::LivesSystem), lives_system);
  system_manager_->set_signature(typeid(ecs::LivesSystem), lives_sig);
}