//
// Created by Jace Shultz (spaceofjace) on 4/21/2025.
//
#include "../include/ecs/EntityManager.h"

#include <gtest/gtest.h>

TEST(EntityManagerTests, CreateEntity_MultipleCreatesHaveUniqueIds)
{
  EntityManager em;

  Entity a = em.CreateEntity();
  Entity b = em.CreateEntity();
  Entity c = em.CreateEntity();
  Entity d = em.CreateEntity();

  EXPECT_NE(a, b);
  EXPECT_NE(a, c);
  EXPECT_NE(a, d);
  EXPECT_NE(b, c);
  EXPECT_NE(b, d);
  EXPECT_NE(c, d);
}

TEST(EntityManagerTests, DestroyEntity_RemovesFromAliveSet) {
  EntityManager em;
  Entity e = em.CreateEntity();
  EXPECT_TRUE(em.IsAlive(e));

  em.DestroyEntity(e);
  EXPECT_FALSE(em.IsAlive(e));
}

TEST(EntityManagerTests, IsAlive_ReturnsFalseForUnknownEntityId) {
  EntityManager em;
  EXPECT_FALSE(em.IsAlive(Entity{ 12345, 1000})); // No entities were created
}

TEST(EntityManagerTests, ClearAll_EmptiesEntityList) {
  EntityManager em;
  em.CreateEntity();
  em.CreateEntity();

  em.ClearAll();
  EXPECT_TRUE(em.GetAllEntities().empty());
}

TEST(EntityManagerTests, GetAllEntities_ReturnsActiveEntities) {
  EntityManager em;
  Entity a = em.CreateEntity();
  Entity b = em.CreateEntity();

  const auto& entities = em.GetAllEntities();
  EXPECT_EQ(entities.size(), 2);
  EXPECT_TRUE(entities.find(a) != entities.end());
  EXPECT_TRUE(entities.find(b)!= entities.end());
}

TEST(EntityManagerTests, CreateEntity_DoesNotReuseIdsByDefault) {
  EntityManager em(false);
  Entity a = em.CreateEntity();
  em.DestroyEntity(a);
  Entity b = em.CreateEntity();

  EXPECT_NE(a.id, b.id); // Should not be reused
}

TEST(EntityManagerTests, ReusedEntityId_HasIncrementedVersion) {
  EntityManager em(true);
  Entity a = em.CreateEntity();
  em.DestroyEntity(a);
  Entity b = em.CreateEntity();

  EXPECT_EQ(a.id, b.id);
  EXPECT_NE(a.version, b.version);
}

TEST(EntityManagerTests, ReusePool_HonorsMaxReusePoolSize) {
  EntityManager em(true, 1); // Only one ID can be reused

  Entity a = em.CreateEntity();
  Entity b = em.CreateEntity();
  em.DestroyEntity(a);
  em.DestroyEntity(b);

  Entity c = em.CreateEntity();
  Entity d = em.CreateEntity();

  // Only one should match a destroyed ID
  bool reusedOne = (c.id == a.id || c.id == b.id);
  bool reusedAnother = (d.id == a.id || d.id == b.id);

  EXPECT_TRUE(reusedOne);
  EXPECT_FALSE(reusedOne && reusedAnother); // Only one reuse allowed
}