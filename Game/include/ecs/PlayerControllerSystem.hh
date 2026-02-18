#pragma once
#include "System.hh"
#include "Registry.hh"

namespace Game {
  // TODO: PlayerControllerSystem handles player movement, rotation, and physics
  // It reads input from PlayerControllerComponent and updates TransformComponent

  class PlayerControllerSystem : public ECS::System {
  public:
    // TODO: Constructor - simple, no parameters needed
    PlayerControllerSystem();

    // TODO: Main update loop - called every frame
    // Reads input, applies movement, handles gravity/jumping
    void Update(ECS::Registry& registry, float deltaTime) override;
  };
}
