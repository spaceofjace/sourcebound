//
// Created by Jace Shultz (spaceofjace) on 4/21/2025.
//
#include <gtest/gtest.h>
#include "../include/ecs/EntityManager.h"

using namespace sb::ecs;

TEST(EntityManagerTests, CreateEntity_MultipleCreatesHaveUniqueIds) {
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

TEST(EntityManagerTests, DestroyEntity_RemovesFromAliveSet) {
  EntityManager em;
  Entity e = em.create_entity();
  EXPECT_TRUE(em.is_alive(e));

  em.destroy_entity(e);
  EXPECT_FALSE(em.is_alive(e));
}

TEST(EntityManagerTests, IsAlive_ReturnsFalseForUnknownEntityId) {
  EntityManager em;
  EXPECT_FALSE(em.is_alive(Entity{ 12345, 1000})); // No entities were created
}

TEST(EntityManagerTests, ClearAll_EmptiesEntityList) {
  EntityManager em;
  em.create_entity();
  em.create_entity();

  em.clear_all();
  EXPECT_TRUE(em.get_all_entities().empty());
}

TEST(EntityManagerTests, GetAllEntities_ReturnsActiveEntities) {
  EntityManager em;
  Entity a = em.create_entity();
  Entity b = em.create_entity();

  const auto& entities = em.get_all_entities();
  EXPECT_EQ(entities.size(), 2);
  EXPECT_TRUE(entities.find(a) != entities.end());
  EXPECT_TRUE(entities.find(b)!= entities.end());
}

TEST(EntityManagerTests, CreateEntity_DoesNotReuseIdsByDefault) {
  EntityManager em(false);
  Entity a = em.create_entity();
  em.destroy_entity(a);
  Entity b = em.create_entity();

  EXPECT_NE(a.id, b.id); // Should not be reused
}

TEST(EntityManagerTests, ReusedEntityId_HasIncrementedVersion) {
  EntityManager em(true);
  Entity a = em.create_entity();
  em.destroy_entity(a);
  Entity b = em.create_entity();

  EXPECT_EQ(a.id, b.id);
  EXPECT_NE(a.version, b.version);
}

TEST(EntityManagerTests, ReusePool_HonorsMaxReusePoolSize) {
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