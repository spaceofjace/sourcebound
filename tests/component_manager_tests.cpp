//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>
#include "../include/ecs/Entity.h"
#include "../include/ecs/IComponentManager.h"

struct Position {
  float x, y;
};

struct Velocity {
  float dx, dy;
};

TEST(ComponentManagerTest, CanRegisterComponentType) {
  ComponentManager cm;
  EXPECT_NO_THROW(cm.register_component<Position>());
}

TEST(ComponentManagerTest, CanAddAndRetrieveComponent) {
  ComponentManager cm;
  cm.register_component<Position>();

  Entity entity{1, 0};
  cm.add_component<Position>(entity, {5.0f, 10.0f});

  auto pos = cm.get_component<Position>(entity);
  EXPECT_FLOAT_EQ(pos.x, 5.0f);
  EXPECT_FLOAT_EQ(pos.y, 10.0f);
}

// TODO: Will need to replace test with logging functionality once added
TEST(ComponentManagerTest, DuplicateRegistrationTriggersAssert) {
  ComponentManager cm;
  cm.register_component<Position>();
  EXPECT_DEATH(cm.register_component<Position>(), "Component type already registered!");
}

// TODO: Will need to replace test with logging functionality once added
TEST(ComponentManagerTest, AddComponentBeforeRegisterTriggersAssert) {
  ComponentManager cm;
  Entity entity{2, 0};

  EXPECT_DEATH(cm.add_component<Velocity>(entity, {1.0f, 1.0f}),
               "Component type not registered before use");
}

// TODO: Will need to replace test with logging functionality once added
TEST(ComponentManagerTest, GetAfterRemoveTriggersAssert) {
  ComponentManager cm;
  cm.register_component<Position>();

  Entity entity{3, 0};
  cm.add_component<Position>(entity, {9.0f, 4.0f});
  cm.remove_component<Position>(entity);

  EXPECT_FALSE(cm.has_component<Position>(entity));
  EXPECT_DEATH(cm.get_component<Position>(entity),
               "Attempted to get a component that doesn't exist.");
}

TEST(ComponentManagerTest, HasComponentIsCorrect) {
  ComponentManager cm;
  cm.register_component<Position>();

  Entity entity{4, 0};
  EXPECT_FALSE(cm.has_component<Position>(entity));
  cm.add_component<Position>(entity, {0.0f, 0.0f});
  EXPECT_TRUE(cm.has_component<Position>(entity));
}

TEST(ComponentManagerTest, EntityDestroyedRemovesAllComponents) {
  ComponentManager cm;
  cm.register_component<Position>();
  cm.register_component<Velocity>();

  Entity entity{5, 0};
  cm.add_component<Position>(entity, {1.0f, 2.0f});
  cm.add_component<Velocity>(entity, {3.0f, 4.0f});

  cm.entity_destroyed(entity);

  EXPECT_FALSE(cm.has_component<Position>(entity));
  EXPECT_FALSE(cm.has_component<Velocity>(entity));
}

TEST(ComponentManagerTest, HandlesMultipleEntitiesAndComponents) {
  ComponentManager cm;
  cm.register_component<Position>();
  cm.register_component<Velocity>();

  Entity e1{10, 0};
  Entity e2{11, 0};

  cm.add_component<Position>(e1, {1.0f, 2.0f});
  cm.add_component<Velocity>(e1, {3.0f, 4.0f});
  cm.add_component<Position>(e2, {5.0f, 6.0f});

  EXPECT_FLOAT_EQ(cm.get_component<Position>(e2).x, 5.0f);
  EXPECT_TRUE(cm.has_component<Velocity>(e1));
  EXPECT_FALSE(cm.has_component<Velocity>(e2));
}
