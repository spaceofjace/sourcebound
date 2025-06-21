//
// Created by Jace Shultz on 4/21/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//
#include <memory>
#include <gtest/gtest.h>

#include "../include/ecs/EntityManager.h"
#include "mocks/MockSink.h"
#include "../include/logger/Logger.h"

using namespace sb::ecs;

TEST(EntityManagerTest, CreateEntity_MultipleCreatesHaveUniqueIds) {
  EntityManager em;

  Entity a = em.create_entity();
  Entity b = em.create_entity();
  Entity c = em.create_entity();
  Entity d = em.create_entity();

  EXPECT_NE(a, b);
  EXPECT_NE(a, c);
  EXPECT_NE(a, d);
  EXPECT_NE(b, c);
  EXPECT_NE(b, d);
  EXPECT_NE(c, d);
}

TEST(EntityManagerTest, CreateEntity_ReturnsNullWhenMaxReached) {
  auto mock_sink = std::make_shared<MockSink>();
  Logger::set_sink(mock_sink);

  EntityManager em;
  for (std::size_t i = 0; i < MAX_ENTITIES; ++i) {
    em.create_entity();
  }

  Entity result = em.create_entity();
  EXPECT_TRUE(result.is_null());

  EXPECT_EQ(mock_sink->last_level, sb::log::Level::Error);
  EXPECT_EQ(mock_sink->last_message, "[EntityManager] Max entities reached!");
}

TEST(EntityManagerTest, DestroyEntity_RemovesFromAliveSet) {
  EntityManager em;
  Entity e = em.create_entity();
  EXPECT_TRUE(em.is_alive(e));

  em.destroy_entity(e);
  EXPECT_FALSE(em.is_alive(e));
}

TEST(EntityManagerTest, IsAlive_ReturnsFalseForUnknownEntityId) {
  EntityManager em;
  EXPECT_FALSE(em.is_alive(Entity{ 12345, 1000})); // No entities were created
}

TEST(EntityManagerTest, IsAlive_ReturnsFalseIfVersionMismatch) {
  EntityManager em(true);
  Entity a = em.create_entity();
  em.destroy_entity(a);

  Entity b = em.create_entity();  // Should reuse e1.id, but increment version
  
  EXPECT_FALSE(em.is_alive(a));  // Same ID, old version
  EXPECT_TRUE(em.is_alive(b));   // Correct ID and version
}

TEST(EntityManagerTest, ClearAll_EmptiesEntityList) {
  EntityManager em;
  em.create_entity();
  em.create_entity();

  em.clear_all();
  EXPECT_TRUE(em.get_all_entities().empty());
}

TEST(EntityManagerTest, GetAllEntities_ReturnsActiveEntities) {
  EntityManager em;
  Entity a = em.create_entity();
  Entity b = em.create_entity();

  const auto& entities = em.get_all_entities();
  EXPECT_EQ(entities.size(), 2);
  EXPECT_TRUE(entities.find(a) != entities.end());
  EXPECT_TRUE(entities.find(b)!= entities.end());
}

TEST(EntityManagerTest, CreateEntity_DoesNotReuseIdsByDefault) {
  EntityManager em(false);
  Entity a = em.create_entity();
  em.destroy_entity(a);
  Entity b = em.create_entity();

  EXPECT_NE(a.id, b.id); // Should not be reused
}

TEST(EntityManagerTest, ReusedEntityId_HasIncrementedVersion) {
  EntityManager em(true);
  Entity a = em.create_entity();
  em.destroy_entity(a);
  Entity b = em.create_entity();

  EXPECT_EQ(a.id, b.id);
  EXPECT_NE(a.version, b.version);
}

TEST(EntityManagerTest, ReusePool_HonorsMaxReusePoolSize) {
  EntityManager em(true, 1); // Only one ID can be reused

  Entity a = em.create_entity();
  Entity b = em.create_entity();
  em.destroy_entity(a);
  em.destroy_entity(b);

  Entity c = em.create_entity();
  Entity d = em.create_entity();

  // Only one should match a destroyed ID
  bool reusedOne = (c.id == a.id || c.id == b.id);
  bool reusedAnother = (d.id == a.id || d.id == b.id);

  EXPECT_TRUE(reusedOne);
  EXPECT_FALSE(reusedOne && reusedAnother); // Only one reuse allowed
}

TEST(EntityManagerTest, DestroyEntity_ErasesSignatureIfPresent) {
  EntityManager em;
  Entity e = em.create_entity();

  Signature sig;
  sig.set(3);
  em.set_signature(e.id, sig);

  EXPECT_TRUE(em.destroy_entity(e)); // Should return true
  Signature out;
  EXPECT_FALSE(em.try_get_signature(e.id, out)); // Signature should be gone
}

TEST(EntityManagerTest, DestroyEntity_LogsWarningIfEntityNotFound) {
  auto mock_sink = std::make_shared<MockSink>();
  Logger::set_sink(mock_sink);

  EntityManager em;
  Entity fake{9999, 99};

  bool destroyed = em.destroy_entity(fake);
  EXPECT_FALSE(destroyed);

  EXPECT_EQ(mock_sink->last_level, sb::log::Level::Warning);
  EXPECT_EQ(mock_sink->last_message, "[EntityManager] Attempted to destroy non-existent entity.");
}

TEST(EntityManagerTest, TryGetSignature_ReturnsFalseIfNotSet) {
  auto mock_sink = std::make_shared<MockSink>();
  Logger::set_sink(mock_sink);

  EntityManager em;
  Entity e = em.create_entity();

  em.destroy_entity(e); // remove it

  Signature sig;
  bool result = em.try_get_signature(e.id, sig);
  EXPECT_FALSE(result);
  EXPECT_EQ(mock_sink->last_level, sb::log::Level::Warning);
  EXPECT_EQ(mock_sink->last_message, "[EntityManager] Signature not set!");
}

TEST(EntityManagerTest, SetSignature_LogsWarningIfSameSignatureIsSetAgain) {
  auto mock_sink = std::make_shared<MockSink>();
  Logger::set_sink(mock_sink);

  EntityManager em;
  Entity e = em.create_entity();

  Signature sig;
  sig.set(2);
  em.set_signature(e.id, sig);     // first set: OK
  em.set_signature(e.id, sig);     // second set: logs warning

  EXPECT_EQ(mock_sink->last_level, sb::log::Level::Warning);
  EXPECT_EQ(mock_sink->last_message, "[EntityManager] Signature already set!");
}

TEST(EntityManagerTest, GetEntitiesWithSignature_ReturnsMatchingEntities) {
  EntityManager em;
  Entity a = em.create_entity();
  Entity b = em.create_entity();
  Entity c = em.create_entity();

  Signature match;
  match.set(1);
  match.set(3);

  em.set_signature(a.id, match);
  em.set_signature(b.id, match);

  Signature non_match;
  non_match.set(1);
  em.set_signature(c.id, non_match);

  std::vector<Entity> result = em.get_entities_with_signature(match);

  EXPECT_EQ(result.size(), 2);
  EXPECT_NE(std::find(result.begin(), result.end(), a), result.end());
  EXPECT_NE(std::find(result.begin(), result.end(), b), result.end());
  EXPECT_EQ(std::find(result.begin(), result.end(), c), result.end());
}
TEST(EntityManagerTest, GetEntitiesWithSignature_EmptyIfNoMatch) {
  EntityManager em;
  Entity e = em.create_entity();

  Signature sig;
  sig.set(2);
  em.set_signature(e.id, sig);

  Signature noMatch;
  noMatch.set(0);  // different bit

  std::vector<Entity> result = em.get_entities_with_signature(noMatch);

  EXPECT_TRUE(result.empty());
}

TEST(EntityManagerTest, GetEntitiesWithSignature_ReturnsEmptyWhenNoSignaturesSet) {
  EntityManager em;
  em.create_entity();
  em.create_entity();

  Signature target_signature;
  target_signature.set(0);

  std::vector<Entity> result = em.get_entities_with_signature(target_signature);

  EXPECT_TRUE(result.empty());
}