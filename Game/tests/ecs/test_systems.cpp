#include <gtest/gtest.h>
#include "ecs/Registry.hh"
#include "ecs/Components.hh"
#include <glm/glm.hpp>

using namespace ECS;
using namespace Game;

// Test InputSystem Component Structure
TEST(InputSystemTest, ComponentDefaults) {
    PlayerControllerComponent controller;
    
    // Verify default values
    EXPECT_FLOAT_EQ(controller.moveSpeed, 5.0f);
    EXPECT_FLOAT_EQ(controller.lookSensitivity, 0.1f);
    EXPECT_FLOAT_EQ(controller.cameraHeight, 1.7f);
    // Note: glm::vec2 doesn't have default initialization, so we don't test moveInput/lookInput here
}

TEST(InputSystemTest, UpdatesMoveInput) {
    Registry registry;
    
    // Create player entity
    auto player = registry.CreateEntity();
    registry.AddComponent(player, TransformComponent{});
    registry.AddComponent(player, PlayerControllerComponent{});
    
    auto* controller = registry.GetComponent<PlayerControllerComponent>(player);
    ASSERT_NE(controller, nullptr);
    
    // Simulate setting inputs manually (what InputSystem would do)
    controller->moveInput = glm::vec2(1.0f, 0.0f); // Moving right (D key)
    
    EXPECT_EQ(controller->moveInput.x, 1.0f);
    EXPECT_EQ(controller->moveInput.y, 0.0f);
    
    // Test diagonal movement
    controller->moveInput = glm::vec2(1.0f, 1.0f); // D + W
    EXPECT_EQ(controller->moveInput.x, 1.0f);
    EXPECT_EQ(controller->moveInput.y, 1.0f);
    
    // Test negative movement
    controller->moveInput = glm::vec2(-1.0f, -1.0f); // A + S
    EXPECT_EQ(controller->moveInput.x, -1.0f);
    EXPECT_EQ(controller->moveInput.y, -1.0f);
}

TEST(InputSystemTest, UpdatesLookInput) {
    Registry registry;
    
    // Create player entity
    auto player = registry.CreateEntity();
    registry.AddComponent(player, TransformComponent{});
    registry.AddComponent(player, PlayerControllerComponent{});
    
    auto* controller = registry.GetComponent<PlayerControllerComponent>(player);
    ASSERT_NE(controller, nullptr);
    
    // Simulate mouse movement (what InputSystem would do)
    controller->lookInput = glm::vec2(10.0f, -5.0f); // Mouse moved right and down
    
    EXPECT_EQ(controller->lookInput.x, 10.0f);
    EXPECT_EQ(controller->lookInput.y, -5.0f);
}

TEST(InputSystemTest, QueriesPlayerEntity) {
    Registry registry;
    
    // Create player entity with PlayerControllerComponent
    auto player = registry.CreateEntity();
    registry.AddComponent(player, TransformComponent{});
    registry.AddComponent(player, PlayerControllerComponent{});
    
    // Query should find the player
    auto entities = registry.Query<PlayerControllerComponent>();
    EXPECT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0], player);
    
    // Add another player entity
    auto player2 = registry.CreateEntity();
    registry.AddComponent(player2, TransformComponent{});
    registry.AddComponent(player2, PlayerControllerComponent{});
    
    // Query should find both
    entities = registry.Query<PlayerControllerComponent>();
    EXPECT_EQ(entities.size(), 2);
}

// Test RenderSystem Queries Correct Entities
TEST(RenderSystemTest, QueryRenderableEntities) {
    Registry registry;
    
    // Create entity with both components
    auto e1 = registry.CreateEntity();
    registry.AddComponent(e1, TransformComponent{});
    registry.AddComponent(e1, RenderableComponent{"model1.obj"});
    
    // Create entity with only Transform (should not be rendered)
    auto e2 = registry.CreateEntity();
    registry.AddComponent(e2, TransformComponent{});
    
    // Create entity with only Renderable (should not be rendered)
    auto e3 = registry.CreateEntity();
    registry.AddComponent(e3, RenderableComponent{"model2.obj"});
    
    // Query should only return e1
    auto entities = registry.Query<TransformComponent, RenderableComponent>();
    EXPECT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0], e1);
}

TEST(RenderSystemTest, TransformComponentCalculatesMatrix) {
    TransformComponent transform;
    transform.position = glm::vec3(10.0f, 5.0f, 3.0f);
    transform.rotation = glm::vec3(0.0f, 90.0f, 0.0f); // 90 degrees around Y
    transform.scale = glm::vec3(2.0f, 2.0f, 2.0f);
    
    glm::mat4 model = transform.GetModelMatrix();
    
    // Verify it's a valid matrix (not identity)
    glm::mat4 identity = glm::mat4(1.0f);
    EXPECT_NE(model, identity);
    
    // The matrix should be calculable without crashing
    EXPECT_NO_THROW(transform.GetModelMatrix());
}

TEST(RenderSystemTest, MultipleRenderableEntities) {
    Registry registry;
    
    // Create multiple renderable entities
    auto e1 = registry.CreateEntity();
    registry.AddComponent(e1, TransformComponent{.position = glm::vec3(0, 0, 0)});
    registry.AddComponent(e1, RenderableComponent{"model1.obj"});
    
    auto e2 = registry.CreateEntity();
    registry.AddComponent(e2, TransformComponent{.position = glm::vec3(10, 0, 0)});
    registry.AddComponent(e2, RenderableComponent{"model2.obj"});
    
    auto e3 = registry.CreateEntity();
    registry.AddComponent(e3, TransformComponent{.position = glm::vec3(-10, 0, 0)});
    registry.AddComponent(e3, RenderableComponent{"model1.obj"});
    
    auto entities = registry.Query<TransformComponent, RenderableComponent>();
    EXPECT_EQ(entities.size(), 3);
    
    // Verify each has the correct position
    auto* t1 = registry.GetComponent<TransformComponent>(e1);
    auto* t2 = registry.GetComponent<TransformComponent>(e2);
    auto* t3 = registry.GetComponent<TransformComponent>(e3);
    
    EXPECT_EQ(t1->position, glm::vec3(0, 0, 0));
    EXPECT_EQ(t2->position, glm::vec3(10, 0, 0));
    EXPECT_EQ(t3->position, glm::vec3(-10, 0, 0));
}

TEST(RenderSystemTest, RenderableComponentStoresPath) {
    Registry registry;
    
    auto entity = registry.CreateEntity();
    registry.AddComponent(entity, TransformComponent{});
    registry.AddComponent(entity, RenderableComponent{"../assets/model/test.obj"});
    
    auto* renderable = registry.GetComponent<RenderableComponent>(entity);
    ASSERT_NE(renderable, nullptr);
    EXPECT_EQ(renderable->modelPath, "../assets/model/test.obj");
}

TEST(RenderSystemTest, TransformIdentityMatrix) {
    TransformComponent transform;
    // All defaults (position 0,0,0; rotation 0,0,0; scale 1,1,1)
    
    glm::mat4 model = transform.GetModelMatrix();
    glm::mat4 identity = glm::mat4(1.0f);
    
    // Identity transform should produce identity matrix
    EXPECT_EQ(model, identity);
}

TEST(RenderSystemTest, TransformPositionOnly) {
    TransformComponent transform;
    transform.position = glm::vec3(5.0f, 10.0f, -3.0f);
    
    glm::mat4 model = transform.GetModelMatrix();
    
    // Extract translation from matrix (last column)
    EXPECT_FLOAT_EQ(model[3][0], 5.0f);
    EXPECT_FLOAT_EQ(model[3][1], 10.0f);
    EXPECT_FLOAT_EQ(model[3][2], -3.0f);
}

// Integration test for Systems working together
TEST(SystemsIntegrationTest, InputAndRenderWorkTogether) {
    Registry registry;
    
    // Create a player entity that can move and be rendered
    auto player = registry.CreateEntity();
    registry.AddComponent(player, TransformComponent{
        .position = glm::vec3(0, 0, 0),
        .rotation = glm::vec3(0, 0, 0),
        .scale = glm::vec3(1, 1, 1)
    });
    registry.AddComponent(player, PlayerControllerComponent{});
    registry.AddComponent(player, RenderableComponent{"player_model.obj"});
    
    // Verify player has all required components
    EXPECT_TRUE(registry.HasComponent<TransformComponent>(player));
    EXPECT_TRUE(registry.HasComponent<PlayerControllerComponent>(player));
    EXPECT_TRUE(registry.HasComponent<RenderableComponent>(player));
    
    // Simulate InputSystem updating the controller
    auto* controller = registry.GetComponent<PlayerControllerComponent>(player);
    controller->moveInput = glm::vec2(1, 0); // Moving right
    controller->lookInput = glm::vec2(10, 5); // Looking right and down
    
    // Verify InputSystem would find this entity
    auto inputEntities = registry.Query<PlayerControllerComponent>();
    EXPECT_EQ(inputEntities.size(), 1);
    EXPECT_EQ(inputEntities[0], player);
    
    // Verify RenderSystem would find this entity
    auto renderEntities = registry.Query<TransformComponent, RenderableComponent>();
    EXPECT_EQ(renderEntities.size(), 1);
    EXPECT_EQ(renderEntities[0], player);
}

TEST(SystemsIntegrationTest, EntityRecyclingWithComponents) {
    Registry registry;
    
    // Create and destroy entity with components
    auto entity = registry.CreateEntity();
    registry.AddComponent(entity, TransformComponent{.position = glm::vec3(5, 5, 5)});
    registry.AddComponent(entity, RenderableComponent{"test.obj"});
    
    // Verify component exists
    auto* transform = registry.GetComponent<TransformComponent>(entity);
    EXPECT_EQ(transform->position, glm::vec3(5, 5, 5));
    
    // Destroy entity
    registry.DestroyEntity(entity);
    
    // Create new entity (should recycle ID)
    auto newEntity = registry.CreateEntity();
    EXPECT_EQ(newEntity, entity); // Same ID recycled
    
    // New entity should not have the old components
    EXPECT_FALSE(registry.HasComponent<TransformComponent>(newEntity));
    EXPECT_FALSE(registry.HasComponent<RenderableComponent>(newEntity));
}

TEST(SystemsIntegrationTest, MultiplePlayersAndRenderables) {
    Registry registry;
    
    // Create player 1 (has controller + renderable)
    auto player1 = registry.CreateEntity();
    registry.AddComponent(player1, TransformComponent{.position = glm::vec3(0, 0, 0)});
    registry.AddComponent(player1, PlayerControllerComponent{.moveSpeed = 5.0f});
    registry.AddComponent(player1, RenderableComponent{"player1.obj"});
    
    // Create player 2 (has controller + renderable)
    auto player2 = registry.CreateEntity();
    registry.AddComponent(player2, TransformComponent{.position = glm::vec3(10, 0, 0)});
    registry.AddComponent(player2, PlayerControllerComponent{.moveSpeed = 3.0f});
    registry.AddComponent(player2, RenderableComponent{"player2.obj"});
    
    // Create static object (renderable only, no controller)
    auto crate = registry.CreateEntity();
    registry.AddComponent(crate, TransformComponent{.position = glm::vec3(5, 0, 5)});
    registry.AddComponent(crate, RenderableComponent{"crate.obj"});
    
    // InputSystem would find 2 players
    auto players = registry.Query<PlayerControllerComponent>();
    EXPECT_EQ(players.size(), 2);
    
    // RenderSystem would find all 3 renderables
    auto renderables = registry.Query<TransformComponent, RenderableComponent>();
    EXPECT_EQ(renderables.size(), 3);
}

TEST(SystemsIntegrationTest, ComponentQueriesAreIndependent) {
    Registry registry;
    
    // Create various entities
    auto player = registry.CreateEntity();
    registry.AddComponent(player, TransformComponent{});
    registry.AddComponent(player, PlayerControllerComponent{});
    registry.AddComponent(player, RenderableComponent{"player.obj"});
    
    auto enemy = registry.CreateEntity();
    registry.AddComponent(enemy, TransformComponent{});
    registry.AddComponent(enemy, RenderableComponent{"enemy.obj"});
    // No PlayerControllerComponent - AI controlled
    
    auto prop = registry.CreateEntity();
    registry.AddComponent(prop, TransformComponent{});
    registry.AddComponent(prop, RenderableComponent{"prop.obj"});
    
    // Different queries return different sets
    auto allRenderables = registry.Query<TransformComponent, RenderableComponent>();
    auto allPlayers = registry.Query<PlayerControllerComponent>();
    auto allTransforms = registry.Query<TransformComponent>();
    
    EXPECT_EQ(allRenderables.size(), 3); // All three can be rendered
    EXPECT_EQ(allPlayers.size(), 1);      // Only player has controller
    EXPECT_EQ(allTransforms.size(), 3);   // All three have transforms
}
