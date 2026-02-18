#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <cstdint>

namespace ECS {
  using EntityID = uint32_t;
  using ComponentID = uint32_t;
  constexpr EntityID INVALID_ENTITY = 0xFFFFFFFF;
  constexpr uint32_t MAX_COMPONENTS = 32; // Bitmask size
}
