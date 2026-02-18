#include <glm/glm.hpp>
#include "ecs/CameraSystem.hh"

namespace Game {
  CameraSystem::CameraSystem(Camera* camera) : m_Camera(camera) {}

  void CameraSystem::Update(ECS::Registry& registry, float deltaTime)
  {
    // Query for player entity
    auto entities = registry.Query<TransformComponent, PlayerControllerComponent>();
    if (entities.empty()) return;
    auto entity = entities[0];
    auto* transform = registry.GetComponent<TransformComponent>(entity);
    auto* controller = registry.GetComponent<PlayerControllerComponent>(entity);

    // Position camera at eye level
    m_Camera->Position = transform->position + glm::vec3(0.0f, controller->cameraHeight, 0.0f);

    // Update rotation from player
    m_Camera->Yaw = transform->rotation.y;
    m_Camera->Pitch = transform->rotation.x;

    // Clamp to stop flipping
    if (m_Camera->Pitch > 89.0f) { m_Camera->Pitch = 89.0f; }
    if (m_Camera->Pitch < -89.0f) { m_Camera->Pitch = -89.0f; }

    m_Camera->updateCameraVectors();
  }
}
