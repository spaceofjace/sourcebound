//
// Created by Jace Shultz on 4/22/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>

#include "../include/ecs/SystemManager.h"
#include "../include/logger/Logger.h"
#include "mocks/MockSink.h"

using namespace sb::ecs;

struct MockSystem : ISystem {
  // Placeholder mock system
  // Will likely extract into separate file when more mocking behavior becomes necessary
};

TEST(SystemManagerTest, CanRegisterSystem) {
  SystemManager sm;
  EXPECT_NO_THROW(sm.register_system<MockSystem>());
}

TEST(SystemManagerTest, DuplicateRegistrationTriggersErrorLog) {
  auto mock_sink = std::make_shared<MockSink>();
  Logger::set_sink(mock_sink);

  SystemManager sm;
  sm.register_system<MockSystem>();
  EXPECT_THROW(sm.register_system<MockSystem>(), std::runtime_error); // NOLINT(*-type-traits)

  EXPECT_EQ(mock_sink->last_level, sb::log::Level::Error);
  EXPECT_EQ(mock_sink->last_message, "[SystemManager] System already registered");
}

TEST(SystemManagerTest, SetSignatureForRegisteredSystemSucceeds) {
  SystemManager sm;
  sm.register_system<MockSystem>();

  Signature sig;
  sig.set(1);
  EXPECT_NO_THROW(sm.set_signature<MockSystem>(sig));
}

TEST(SystemManagerTest, SetSignatureBeforeRegisterTriggersErrorLogAndThrows) {
  auto mock_sink = std::make_shared<MockSink>();
  Logger::set_sink(mock_sink);

  SystemManager sm;
  Signature sig;
  sig.set(1);

  EXPECT_THROW(sm.set_signature<MockSystem>(sig), std::runtime_error); // NOLINT(*-type-traits)

  EXPECT_EQ(mock_sink->last_level, sb::log::Level::Error);
  EXPECT_EQ(mock_sink->last_message,
            "[SystemManager] System not registered before setting signature.");
}

TEST(SystemManagerTest, GetRegisteredSystemReturnsCorrectPointer) {
  SystemManager sm;
  auto system = sm.register_system<MockSystem>();
  auto retrieved = sm.get_system<MockSystem>();

  EXPECT_EQ(system, retrieved);
}

TEST(SystemManagerTest, GetUnregisteredSystemTriggersErrorLogAndThrows) {
  auto mock_sink = std::make_shared<MockSink>();
  Logger::set_sink(mock_sink);

  SystemManager sm;

  EXPECT_THROW(sm.get_system<MockSystem>(), std::runtime_error); // NOLINT(*-type-traits)

  EXPECT_EQ(mock_sink->last_level, sb::log::Level::Error);
  EXPECT_EQ(mock_sink->last_message, "[SystemManager] System not registered.");
}

TEST(SystemManagerTest, EntityDestroyedRemovesEntityFromAllSystems) {
  SystemManager sm;
  auto system = sm.register_system<MockSystem>();
  system->entities.insert(Entity{1, 0});
  system->entities.insert(Entity{2, 0});

  sm.entity_destroyed(Entity{1, 0});

  EXPECT_FALSE(system->entities.find(Entity{1, 0}) != system->entities.end());
  EXPECT_TRUE(system->entities.find(Entity{2, 0}) != system->entities.end());
}

TEST(SystemManagerTest, EntitySignatureChangedTracksEntitiesCorrectly) {
  SystemManager sm;
  auto system = sm.register_system<MockSystem>();

  Signature sig;
  sig.set(1);
  sm.set_signature<MockSystem>(sig);

  Entity entity{42, 0};

  // Matching signature
  Signature matching;
  matching.set(1);

  sm.entity_signature_changed(entity, matching);
  EXPECT_TRUE(system->entities.find(entity) != system->entities.end());

  // Non-matching signature
  Signature nonmatching;
  nonmatching.set(2);

  sm.entity_signature_changed(entity, nonmatching);
  EXPECT_FALSE(system->entities.find(entity) != system->entities.end());
}