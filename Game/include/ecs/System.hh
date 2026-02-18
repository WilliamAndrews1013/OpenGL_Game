#pragma once
#include "Registry.hh"

namespace ECS {
  class System {
  public:
    virtual ~System() = default;
    virtual void Update(Registry& registry, float deltaTime) = 0;
    virtual void Initialize(Registry& registry) {}
    virtual void Shutdown(Registry& registry) {}
  };
}
