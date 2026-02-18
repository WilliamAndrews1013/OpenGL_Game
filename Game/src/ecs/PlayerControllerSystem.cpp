#include "ecs/PlayerControllerSystem.hh"
#include "ecs/Components.hh"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace Game {

  // ============================================
  // STEP 0: Constructor Implementation
  // ============================================
  // TODO: Implement simple constructor
  // PlayerControllerSystem::PlayerControllerSystem() {
  //   // Nothing special needed here
  // }

  // ============================================
  // MAIN UPDATE FUNCTION
  // ============================================
  void PlayerControllerSystem::Update(ECS::Registry& registry, float deltaTime)
  {
    // ==========================================
    // STEP 1: Find the player entity
    // ==========================================
    // TODO: Query registry for entities with BOTH components:
    //   - TransformComponent (to update position/rotation)
    //   - PlayerControllerComponent (to read input values)
    // HINT: auto entities = registry.Query<Game::TransformComponent, Game::PlayerControllerComponent>();

    // TODO: Check if entities vector is empty - if so, return early
    // (no player to control)

    // TODO: Get the first entity (we assume single player for now)
    // auto entity = entities[0];

    // ==========================================
    // STEP 2: Get component pointers
    // ==========================================
    // TODO: Get pointer to TransformComponent
    // HINT: auto* transform = registry.GetComponent<Game::TransformComponent>(entity);

    // TODO: Get pointer to PlayerControllerComponent
    // HINT: auto* controller = registry.GetComponent<Game::PlayerControllerComponent>(entity);

    // ==========================================
    // STEP 3: Apply mouse look (rotation)
    // ==========================================
    // TODO: Update YAW (horizontal rotation around Y axis)
    //   transform->rotation.y += controller->lookInput.x * controller->lookSensitivity;
    // This makes mouse X control looking left/right

    // TODO: Update PITCH (vertical rotation around X axis)
    //   transform->rotation.x += controller->lookInput.y * controller->lookSensitivity;
    // This makes mouse Y control looking up/down

    // TODO: Clamp pitch to prevent camera flipping
    //   transform->rotation.x = glm::clamp(transform->rotation.x, -89.0f, 89.0f);
    // Without this, looking straight up/down breaks the camera

    // ==========================================
    // STEP 4: Calculate movement vectors
    // ==========================================
    // TODO: Convert yaw rotation to radians for trig functions
    //   float yaw = glm::radians(transform->rotation.y);

    // TODO: Calculate FORWARD direction (where player is facing, flattened to XZ plane)
    //   glm::vec3 forward = glm::vec3(sin(yaw), 0.0f, cos(yaw));
    // This creates a unit vector pointing in the facing direction
    // Y is 0 because we move on the ground (no flying yet)

    // TODO: Calculate RIGHT direction (perpendicular to forward)
    //   glm::vec3 right = glm::vec3(cos(yaw), 0.0f, -sin(yaw));
    // This is 90 degrees rotated from forward

    // ==========================================
    // STEP 5: Apply horizontal movement
    // ==========================================
    // TODO: Calculate movement amount this frame
    //   glm::vec3 movement = (forward * controller->moveInput.y +
    //                        right * controller->moveInput.x) *
    //                        controller->moveSpeed * deltaTime;
    // moveInput.y is W/S (forward/back), moveInput.x is A/D (left/right)
    // Multiply by speed and deltaTime for frame-rate independent movement

    // TODO: Update position X and Z coordinates
    //   transform->position.x += movement.x;
    //   transform->position.z += movement.z;
    // Don't touch Y here - that's handled by gravity!

    // ==========================================
    // STEP 6: Handle jumping
    // ==========================================
    // TODO: Check if jump was requested AND player is on ground
    //   if (controller->jumpRequested && controller->isGrounded) {
    //     controller->verticalVelocity = controller->jumpForce;  // Push up
    //     controller->isGrounded = false;                        // Now in air
    //     controller->jumpRequested = false;                     // Reset flag
    //   }
    // Note: jumpRequested is set by InputSystem when Space is pressed

    // ==========================================
    // STEP 7: Apply gravity
    // ==========================================
    // TODO: Check if player is NOT grounded (in the air)
    //   if (!controller->isGrounded) {

    // TODO: Apply gravity to vertical velocity
    //     controller->verticalVelocity += controller->gravity * deltaTime;
    // Gravity is negative (pulls down), so velocity becomes more negative

    // TODO: Update Y position
    //     transform->position.y += controller->verticalVelocity * deltaTime;
    // This makes the player fall
    //   }

    // ==========================================
    // STEP 8: Ground detection (simple version)
    // ==========================================
    // TODO: Check if player fell below ground level
    //   if (transform->position.y <= controller->cameraHeight) {
    //     transform->position.y = controller->cameraHeight;      // Snap to ground
    //     controller->verticalVelocity = 0.0f;                    // Stop falling
    //     controller->isGrounded = true;                          // On ground
    //   }
    // This is a placeholder - we'll replace with ray casting later!
    // For now, ground is just a flat plane at y = cameraHeight (1.7)

    // ==========================================
    // BONUS: Optional improvements
    // ==========================================
    // TODO (optional): Clear lookInput and moveInput after processing?
    // This prevents input from persisting across frames
    // controller->lookInput = glm::vec2(0);
    // controller->moveInput = glm::vec2(0);
  }

}
