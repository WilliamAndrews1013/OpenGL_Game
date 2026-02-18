#include <gtest/gtest.h>
#include "ecs/Registry.hh"

using namespace ECS;

struct Position {
  float x, y;
};

struct Velocity {
  float dx, dy;
};

TEST(RegistryTest, CreateAndDestroyEntity) {
  Registry registry;

  auto entity = registry.CreateEntity();
  EXPECT_NE(entity, INVALID_ENTITY);
  EXPECT_TRUE(registry.IsValid(entity));

  registry.DestroyEntity(entity);
  EXPECT_FALSE(registry.IsValid(entity));
}

TEST(RegistryTest, AddComponent) {
  Registry registry;
  auto entity = registry.CreateEntity();

  auto& pos = registry.AddComponent(entity, Position{1.0f, 2.0f});
  EXPECT_EQ(pos.x, 1.0f);

  EXPECT_TRUE(registry.HasComponent<Position>(entity));
}

TEST(RegistryTest, QueryEntities) {
  Registry registry;

  auto e1 = registry.CreateEntity();
  registry.AddComponent(e1, Position{0, 0});
  registry.AddComponent(e1, Velocity{1, 1});

  auto e2 = registry.CreateEntity();
  registry.AddComponent(e2, Position{5, 5});
  // No Velocity

  auto results = registry.Query<Position, Velocity>();
  EXPECT_EQ(results.size(), 1);
  EXPECT_EQ(results[0], e1);
}
