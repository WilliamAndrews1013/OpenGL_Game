#pragma once
#include "System.hh"
#include "Registry.hh"
#include "Components.hh"
#include "game/Camera.h"

namespace Game {
  class CameraSystem : public ECS::System {
  public:
    CameraSystem(Camera* camera);
    void Update(ECS::Registry& registry, float deltaTime) override;

  private:
    Camera* m_Camera;
  };
}
