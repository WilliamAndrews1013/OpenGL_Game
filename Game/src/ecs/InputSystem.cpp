#include <glm/glm.hpp>
#include "ecs/InputSystem.hh"

namespace Game {
  InputSystem::InputSystem(Core::Window* window) : m_Window(window) {}

  void InputSystem::Update(ECS::Registry& registry, float deltaTime)
  {
    glm::vec2 moveInput = glm::vec2(0, 0);

    if (m_Window->IsKeyPressed(GLFW_KEY_W)) { moveInput.y += 1; }
    if (m_Window->IsKeyPressed(GLFW_KEY_S)) { moveInput.y -= 1; }
    if (m_Window->IsKeyPressed(GLFW_KEY_A)) { moveInput.x -= 1; }
    if (m_Window->IsKeyPressed(GLFW_KEY_D)) { moveInput.x += 1; }

    double xpos, ypos;
    GLFWwindow* window = m_Window->GetNativeWindow();
    glfwGetCursorPos(window, &xpos, &ypos);

    if (m_FirstMouse) {
      m_LastMouseX = xpos;
      m_LastMouseY = ypos;
      m_FirstMouse = false;
    }

    double deltaX = xpos - m_LastMouseX;
    double deltaY = ypos - m_LastMouseY;
    m_LastMouseX = xpos;
    m_LastMouseY = ypos;

    auto entities = registry.Query<Game::PlayerControllerComponent>();
    if (entities.empty()) return;
    auto entity = entities[0]; // Assume first entity is a player for now
    auto* controller = registry.GetComponent<Game::PlayerControllerComponent>(entity);

    // Sotre mouse delta and move inputs
    controller->lookInput = glm::vec2(deltaX, -deltaY);
    controller->moveInput = moveInput;
    if (m_Window->IsKeyPressed(GLFW_KEY_SPACE)) { controller->jumpRequested = true; }
  }
}
