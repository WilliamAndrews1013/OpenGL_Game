#pragma once
#include "core/Window.hh"
#include "Registry.hh"
#include "System.hh"
#include "Components.hh"

namespace Game {
  class InputSystem : public ECS::System {
  public:

    InputSystem(Core::Window* window);
    void Update(ECS::Registry& registry, float deltaTime) override;

  private:
    Core::Window* m_Window;

    void ProcessKeyboard(ECS::Registry& registry);
    void ProcessMouse(ECS::Registry& registry);

    // Track mouse
    double m_LastMouseX = 0;
    double m_LastMouseY = 0;
    bool m_FirstMouse = true;
  };

}
