#include <gtest/gtest.h>
#include "ecs/Registry.hh"

using namespace ECS;

struct Position {
    float x, y, z;
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct Velocity {
    float dx, dy, dz;
};

TEST(RegistryCompleteTest, CreateAndDestroyEntity) {
    Registry registry;
    
    // Test creation
    auto entity = registry.CreateEntity();
    EXPECT_NE(entity, INVALID_ENTITY);
    EXPECT_TRUE(registry.IsValid(entity));
    EXPECT_EQ(entity, 0); // First entity should be 0
    
    // Test destruction
    registry.DestroyEntity(entity);
    EXPECT_FALSE(registry.IsValid(entity));
}

TEST(RegistryCompleteTest, CreateMultipleEntities) {
    Registry registry;
    
    auto e1 = registry.CreateEntity();
    auto e2 = registry.CreateEntity();
    auto e3 = registry.CreateEntity();
    
    EXPECT_EQ(e1, 0);
    EXPECT_EQ(e2, 1);
    EXPECT_EQ(e3, 2);
    
    EXPECT_TRUE(registry.IsValid(e1));
    EXPECT_TRUE(registry.IsValid(e2));
    EXPECT_TRUE(registry.IsValid(e3));
}

TEST(RegistryCompleteTest, EntityRecycling) {
    Registry registry;
    
    auto e1 = registry.CreateEntity();
    auto e2 = registry.CreateEntity();
    
    registry.DestroyEntity(e1);
    
    // Create new entity - should recycle e1's ID
    auto e3 = registry.CreateEntity();
    EXPECT_EQ(e3, e1); // Should get recycled ID
    EXPECT_TRUE(registry.IsValid(e3));
}

TEST(RegistryCompleteTest, AddAndGetComponent) {
    Registry registry;
    auto entity = registry.CreateEntity();
    
    // Add component
    Position pos{1.0f, 2.0f, 3.0f};
    auto& added = registry.AddComponent(entity, pos);
    
    EXPECT_EQ(added.x, 1.0f);
    EXPECT_EQ(added.y, 2.0f);
    EXPECT_EQ(added.z, 3.0f);
    
    // Get component
    auto* retrieved = registry.GetComponent<Position>(entity);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->x, 1.0f);
    EXPECT_EQ(retrieved->y, 2.0f);
    EXPECT_EQ(retrieved->z, 3.0f);
}

TEST(RegistryCompleteTest, HasComponent) {
    Registry registry;
    auto entity = registry.CreateEntity();
    
    EXPECT_FALSE(registry.HasComponent<Position>(entity));
    
    registry.AddComponent(entity, Position{0, 0, 0});
    EXPECT_TRUE(registry.HasComponent<Position>(entity));
    
    EXPECT_FALSE(registry.HasComponent<Velocity>(entity));
}

TEST(RegistryCompleteTest, RemoveComponent) {
    Registry registry;
    auto entity = registry.CreateEntity();
    
    registry.AddComponent(entity, Position{1.0f, 2.0f, 3.0f});
    EXPECT_TRUE(registry.HasComponent<Position>(entity));
    
    registry.RemoveComponent<Position>(entity);
    EXPECT_FALSE(registry.HasComponent<Position>(entity));
    
    auto* ptr = registry.GetComponent<Position>(entity);
    EXPECT_EQ(ptr, nullptr);
}

TEST(RegistryCompleteTest, DestroyEntityRemovesAllComponents) {
    Registry registry;
    auto entity = registry.CreateEntity();
    
    registry.AddComponent(entity, Position{1.0f, 2.0f, 3.0f});
    registry.AddComponent(entity, Velocity{0.1f, 0.2f, 0.3f});
    
    EXPECT_TRUE(registry.HasComponent<Position>(entity));
    EXPECT_TRUE(registry.HasComponent<Velocity>(entity));
    
    registry.DestroyEntity(entity);
    
    // Entity should be invalid
    EXPECT_FALSE(registry.IsValid(entity));
    
    // Components should be cleaned up
    auto entity2 = registry.CreateEntity();
    // This should be the recycled entity
    EXPECT_EQ(entity2, entity);
    
    // Should have no components
    EXPECT_FALSE(registry.HasComponent<Position>(entity2));
    EXPECT_FALSE(registry.HasComponent<Velocity>(entity2));
}

TEST(RegistryCompleteTest, MultipleComponentsDifferentEntities) {
    Registry registry;
    
    auto e1 = registry.CreateEntity();
    auto e2 = registry.CreateEntity();
    
    registry.AddComponent(e1, Position{1.0f, 0.0f, 0.0f});
    registry.AddComponent(e2, Position{2.0f, 0.0f, 0.0f});
    
    auto* p1 = registry.GetComponent<Position>(e1);
    auto* p2 = registry.GetComponent<Position>(e2);
    
    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);
    
    EXPECT_EQ(p1->x, 1.0f);
    EXPECT_EQ(p2->x, 2.0f);
}
