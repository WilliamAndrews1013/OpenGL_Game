#include "ecs/RenderSystem.hh"
#include <glm/gtc/matrix_transform.hpp>

namespace Game {
  // Constructor
  RenderSystem::RenderSystem(Shader* shader, Camera* camera) : m_Shader(shader), m_Camera(camera)
  {

  }

  void RenderSystem::Update(ECS::Registry& registry, float deltaTime)
  {
    // Clear color buffer and depth buffer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate shader program
    m_Shader->use();

    // Calculate and set view matrix
    glm::mat4 view = m_Camera->GetViewMatrix();
    m_Shader->setMat4("view", view);

    // Calculate and set projection matrix
    glm::mat4 projection = glm::perspective(
        glm::radians(m_Camera->Zoom), // FOV
        1024.0f / 768.0f,             // Aspect ratio
        0.1f,                      // Near plane
        1000.0f
    );
    m_Shader->setMat4("projection", projection);

    // Use Query function to find entities with both components
    auto entities = registry.Query<Game::TransformComponent, Game::RenderableComponent>();

    // Render each components
    for (auto entity : entities) {
      auto* transform = registry.GetComponent<Game::TransformComponent>(entity);
      auto* renderable = registry.GetComponent<Game::RenderableComponent>(entity);

      // Get model matrix
      glm::mat4 model = transform->GetModelMatrix();
      m_Shader->setMat4("model", model);

      // Get/Cache model and draw
      Graphics::Model* modelMesh = GetOrLoadModel(renderable->modelPath);
      if (modelMesh) {
        modelMesh->Draw(*m_Shader);
      }
    }
  }

  Graphics::Model* RenderSystem::GetOrLoadModel(const std::string& path)
  {
    // Check if already cached
    auto it = m_ModelCache.find(path);
    if (it != m_ModelCache.end()) {
      return it->second.get();
    }

    // Not cached - load it
    auto model = std::make_unique<Graphics::Model>(path.c_str());
    Graphics::Model* ptr = model.get();
    m_ModelCache[path] = std::move(model);
    return ptr;
  }

  void Game::RenderSystem::PreLoadModel(const std::string& path)
  {
    GetOrLoadModel(path);
  }
}
