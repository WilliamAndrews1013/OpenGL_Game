#pragma once
#include "System.hh"
#include "Components.hh"
#include "graphics/Model.h"
#include "graphics/shader_s.h"
#include "game/Camera.h"
#include <unordered_map>
#include <memory>

namespace Game {
  class RenderSystem : public ECS::System {
  public:
    RenderSystem(Shader* shader, Game::Camera* camera);

    void Update(ECS::Registry& registry, float deltaTime) override;

    // Preload model to aviod hitches
    void PreLoadModel(const std::string& path);

  private:
    Shader* m_Shader;
    Game::Camera* m_Camera;

    // Model cache: path -> Model
    std::unordered_map<std::string, std::unique_ptr<Graphics::Model>> m_ModelCache;

    // Helper to get model from cache
    Graphics::Model* GetOrLoadModel(const std::string& path);
  };
}
