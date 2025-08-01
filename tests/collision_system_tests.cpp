//
// Created by Jace Shultz on 7/31/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include "../include/ecs/systems/CollisionSystem.h"
#include "../include/ecs/components/Components.h"
#include "../include/math/MathConsts.h"
#include "mocks/MockComponentManager.h"
#include "mocks/MockGameDataManager.h"
#include <gtest/gtest.h>

using namespace sb::ecs;

TEST(CollisionSystemTest, ClampsEntityAgainstRightWall) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Transform>();
  cm->register_component<BoxCollider>();
  cm->register_component<Direction>();
  cm->register_component<Velocity>();
  cm->register_component<Ball>();
  cm->register_component<Paddle>();
  cm->register_component<CircleCollider>();

  auto mock_data = std::make_shared<MockGameDataManager>();
  auto collision = std::make_shared<CollisionSystem>(mock_data);

  Entity wall{1};
  cm->add_component<Transform>(wall, Transform{{100.0f, 0.0f}, {20.0f, 100.0f}});
  cm->add_component<BoxCollider>(wall, BoxCollider{20.0f, 100.0f, 0, 0, CollisionBehavior::Clamp});
  cm->add_component<Direction>(wall, Direction{1.0f, 0.0f});
  collision->entities.insert(wall);

  Entity paddle{2};
  cm->add_component<Transform>(paddle, Transform{{91.0f, 0.0f}, {10.0f, 10.0f}});
  cm->add_component<BoxCollider>(paddle, BoxCollider{10.0f, 10.0f, 0, 0});
  cm->add_component<Direction>(paddle, Direction{1.0f, 0.0f});  // moving right
  cm->add_component<Paddle>(paddle, Paddle{});  // required to be clampable
  collision->entities.insert(paddle);

  collision->update(0.0f, *cm);

  const auto& result = cm->get_component<Transform>(paddle).position.x;

  EXPECT_FLOAT_EQ(result, 85.0f);

  const float right_edge = result + (10.0f / 2.0f);
  EXPECT_FLOAT_EQ(right_edge, 90.0f);
}

TEST(CollisionSystemTest, NoClampIfNotOverlapping) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Transform>();
  cm->register_component<BoxCollider>();
  cm->register_component<Direction>();
  cm->register_component<Velocity>();
  cm->register_component<Ball>();
  cm->register_component<Paddle>();
  cm->register_component<CircleCollider>();

  auto mock_data = std::make_shared<MockGameDataManager>();
  auto collision = std::make_shared<CollisionSystem>(mock_data);

  Entity wall{1};
  cm->add_component<Transform>(wall, Transform{{200.0f, 0.0f}, {20.0f, 100.0f}});
  cm->add_component<BoxCollider>(wall, BoxCollider{20.0f, 100.0f, 0, 0, CollisionBehavior::Clamp});
  cm->add_component<Direction>(wall, Direction{1.0f, 0.0f});
  collision->entities.insert(wall);

  Entity paddle{2};
  cm->add_component<Transform>(paddle, Transform{{100.0f, 0.0f}, {10.0f, 10.0f}});
  cm->add_component<BoxCollider>(paddle, BoxCollider{10.0f, 10.0f, 0, 0});
  cm->add_component<Direction>(paddle, Direction{1.0f, 0.0f});
  collision->entities.insert(paddle);

  collision->update(0.0f, *cm);

  const auto& result = cm->get_component<Transform>(paddle).position.x;
  EXPECT_FLOAT_EQ(result, 100.0f);  // Should be unchanged
}

TEST(CollisionSystemTest, IgnoresEntitiesWithoutDirection) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Transform>();
  cm->register_component<BoxCollider>();
  cm->register_component<Direction>();
  cm->register_component<Velocity>();
  cm->register_component<Ball>();
  cm->register_component<Paddle>();
  cm->register_component<CircleCollider>();

  auto mock_data = std::make_shared<MockGameDataManager>();
  auto collision = std::make_shared<CollisionSystem>(mock_data);

  Entity wall{1};
  cm->add_component<Transform>(wall, Transform{{100.0f, 0.0f}, {20.0f, 100.0f}});
  cm->add_component<BoxCollider>(wall, BoxCollider{20.0f, 100.0f, 0, 0, CollisionBehavior::Clamp});
  cm->add_component<Direction>(wall, Direction{1.0f, 0.0f});
  collision->entities.insert(wall);

  Entity unclamped{2};
  cm->add_component<Transform>(unclamped, Transform{{95.0f, 0.0f}, {10.0f, 10.0f}});
  cm->add_component<BoxCollider>(unclamped, BoxCollider{10.0f, 10.0f, 0, 0});
  cm->add_component<Direction>(unclamped, Direction{1.0f, 0.0f});
  collision->entities.insert(unclamped);

  collision->update(0.0f, *cm);

  const auto& result = cm->get_component<Transform>(unclamped).position.x;
  EXPECT_FLOAT_EQ(result, 95.0f);  // No movement
}

TEST(CollisionSystemTest, ReflectsCircleBounceAgainstFlatWall) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Transform>();
  cm->register_component<CircleCollider>();
  cm->register_component<BoxCollider>();
  cm->register_component<Direction>();
  cm->register_component<PositionBasedBounce>();

  auto mock_data = std::make_shared<MockGameDataManager>();
  auto collision = std::make_shared<CollisionSystem>(mock_data);

  Entity wall{1};
  cm->add_component<Transform>(wall, Transform{{100.0f, 0.0f}, {20.0f, 20.0f}});
  cm->add_component<BoxCollider>(wall, BoxCollider{20.0f, 20.0f, 0, 0, CollisionBehavior::Bounce});
  collision->entities.insert(wall);

  Entity ball{2};
  cm->add_component<Transform>(ball, Transform{{88.0f, 0.0f}, {10.0f, 10.0f}});
  cm->add_component<CircleCollider>(ball, CircleCollider{5.0f});
  cm->add_component<Direction>(ball, Direction{-1.0f, 0.0f});
  collision->entities.insert(ball);

  collision->update(0.0f, *cm);

  const auto& result = cm->get_component<Direction>(ball);

  //Note: there is clamping logic involved in collision to prevent orbiting
  EXPECT_NEAR(result.x, 0.9798f, 0.001f);
  EXPECT_NEAR(result.y, 0.2f, 0.001f);
}

TEST(CollisionSystemTest, AppliesPositionBasedBounce) {
  auto cm = std::make_shared<ComponentManager>();
  cm->register_component<Transform>();
  cm->register_component<CircleCollider>();
  cm->register_component<BoxCollider>();
  cm->register_component<Direction>();
  cm->register_component<PositionBasedBounce>();

  auto mock_data = std::make_shared<MockGameDataManager>();
  auto collision = std::make_shared<CollisionSystem>(mock_data);

  Entity paddle{1};
  cm->add_component<Transform>(paddle, Transform{{100.0f, 0.0f}, {20.0f, 10.0f}});
  cm->add_component<BoxCollider>(paddle, BoxCollider{20.0f, 10.0f, 0, 0, CollisionBehavior::Bounce});
  cm->add_component<PositionBasedBounce>(paddle, PositionBasedBounce{});
  collision->entities.insert(paddle);

  Entity ball{2};
  cm->add_component<Transform>(ball, Transform{{105.0f, -5.0f}, {5.0f, 5.0f}});
  cm->add_component<CircleCollider>(ball, CircleCollider{5.0f});
  cm->add_component<Direction>(ball, Direction{0.0f, 1.0f});  // moving down
  collision->entities.insert(ball);

  collision->update(0.0f, *cm);

  const auto& result = cm->get_component<Direction>(ball);
  EXPECT_NEAR(result.x, 0.5f, 0.001f);
  EXPECT_NEAR(result.y, -0.866f, 0.001f);
}