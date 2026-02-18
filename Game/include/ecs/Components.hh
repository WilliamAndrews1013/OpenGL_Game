#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace Game {
  struct TransformComponent {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 GetModelMatrix() const {
      // Start with identity matrix
      glm::mat4 model = glm::mat4(1.0f);

      // Apply scale
      model = glm::scale(model, scale);

      // Apply rotations (x,y,z)
      model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0)); // Pitch
      model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0)); // Yaw
      model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1)); // Roll

      // Apply translation
      model = glm::translate(model, position);

      return model;
    }
  };

  struct RenderableComponent {
    std::string modelPath;
    // Resource handle instead of raw pointer
  };

  struct CameraComponent {
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    bool isMainCamera = false;
  };

  struct PlayerControllerComponent {
    float moveSpeed = 5.0f;
    float lookSensitivity = 0.1f;
    float cameraHeight = 1.7;

    glm::vec2 moveInput;
    glm::vec2 lookInput;

    // Physics
    float gravity = -9.81f;
    float jumpForce = 5.0f;
    float verticalVelocity = 0.0f;
    bool isGrounded = false;
    bool jumpRequested = false;
  };

  struct NetworkComponent {
    uint32_t networkID;
    bool isLocalPlayer;

    // For interpolation
    glm::vec3 lastPosition;
    float interpolationAlpha = 0.0f;
  };
}
