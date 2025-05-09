//
// Created by Jace Shultz on 4/24/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

#include <gtest/gtest.h>
#include "../include/ecs/EntityManager.h"
#include "../include/ecs/IComponentManager.h"
#include "../include/ecs/components/Components.h"

using namespace sb::ecs;

// These call patterns will later be added into a GameWorld API
template<typename T>
void add_with_signature(EntityManager& em, ComponentManager& cm, const Entity entity, const T& component) {
  cm.add_component<T>(entity, component);
  Signature signature = em.get_signature(entity.id);
  signature.set(cm.get_component_type<T>());
  em.set_signature(entity.id, signature);
}

template<typename T>
void remove_with_signature(EntityManager& em, ComponentManager& cm, const Entity entity) {
  cm.remove_component<T>(entity);
  Signature signature = em.get_signature(entity.id);
  signature.reset(cm.get_component_type<T>());
  em.set_signature(entity.id, signature);
}

// === TESTS ===
TEST(ECSIntegration, CreatePaddleEntity_WithTransformAndTag) {
  EntityManager em = EntityManager();
  auto paddle = em.create_entity();

  ComponentManager cm = ComponentManager();
  cm.register_component<Transform>();
  cm.register_component<Paddle>();

  add_with_signature(em, cm, paddle, Paddle{});

  auto transform = Transform{
    Position{10, 10},
    Velocity{0, 0},
    Size{ 20,5 }
  };
  add_with_signature(em, cm, paddle, transform);

  // Entity is alive
  EXPECT_TRUE(em.is_alive(paddle));

  // ComponentManager has registered both components
  EXPECT_TRUE(cm.has_component<Paddle>(paddle));
  EXPECT_TRUE(cm.has_component<Transform>(paddle));

  // Signature includes both component bits
  Signature final_sig = em.get_signature(paddle.id);
  EXPECT_TRUE(final_sig.test(cm.get_component_type<Paddle>()));
  EXPECT_TRUE(final_sig.test(cm.get_component_type<Transform>()));

  // Component data is correct
  const Transform& t = cm.get_component<Transform>(paddle);
  EXPECT_EQ(t.position.x, 10);
  EXPECT_EQ(t.position.y, 10);
  EXPECT_EQ(t.size.width, 20);
  EXPECT_EQ(t.size.height, 5);
  EXPECT_EQ(t.velocity.x, 0);
  EXPECT_EQ(t.velocity.y, 0);
}

TEST(ECSIntegration, EntityReuse_ChangesVersionAndResetsComponents) {
  EntityManager em(true); 
  ComponentManager cm;

  cm.register_component<Transform>();

  Entity a = em.create_entity();
  auto transform = Transform{
    Position{10, 10},
    Velocity{0, 0},
    Size{ 20,5 }
  };
  add_with_signature(em, cm, a, transform);

  em.destroy_entity(a);
  EXPECT_FALSE(em.is_alive(a));

  Entity b = em.create_entity();
  EXPECT_TRUE(em.is_alive(b));
  EXPECT_NE(a.version, b.version);
  EXPECT_EQ(a.id, b.id); // same ID reused

  EXPECT_FALSE(cm.has_component<Transform>(b));
}

TEST(ECSIntegration, SignatureUpdatedAfterComponentRemoval) {
  EntityManager em;
  ComponentManager cm;

  cm.register_component<Ball>();
  cm.register_component<Transform>();

  Entity ball = em.create_entity();
  add_with_signature(em, cm, ball, Ball{});
  auto transform = Transform{
    Position{0, 0},
    Velocity{1, 0},
    Size{ 20,5 }
  };
  add_with_signature(em, cm, ball, transform);

  Signature s = em.get_signature(ball.id);
  EXPECT_TRUE(s.test(cm.get_component_type<Ball>()));
  EXPECT_TRUE(s.test(cm.get_component_type<Transform>()));

  remove_with_signature<Ball>(em, cm, ball);
  Signature updated = em.get_signature(ball.id);
  EXPECT_FALSE(updated.test(cm.get_component_type<Ball>()));
  EXPECT_TRUE(updated.test(cm.get_component_type<Transform>()));
}

TEST(ECSIntegration, DestroyedEntityRemovesAllComponents) {
  EntityManager em(true);
  ComponentManager cm;

  cm.register_component<Transform>();
  cm.register_component<Collider>();

  Entity e = em.create_entity();
  auto transform = Transform{
    Position{1, 2},
    Velocity{0, 0},
    Size{ 5,5 }
  };
  add_with_signature(em, cm, e, transform);
  add_with_signature(em, cm, e, Collider{true});

  em.destroy_entity(e);
  cm.entity_destroyed(e);

  EXPECT_FALSE(cm.has_component<Transform>(e));
  EXPECT_FALSE(cm.has_component<Collider>(e));
  EXPECT_FALSE(em.is_alive(e));
}