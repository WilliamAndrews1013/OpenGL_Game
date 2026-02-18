#pragma once

#include "core/Window.hh"
#include "network/NetworkClient.hh"
#include "graphics/shader_s.h"
#include "game/Camera.h"
#include "ecs/Registry.hh"
#include "ecs/RenderSystem.hh"
#include "ecs/InputSystem.hh"
#include "ecs/CameraSystem.hh"
#include <memory>

namespace Core {

	class Application {
	public:

		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Network::NetworkClient> m_NetworkClient;
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Game::Camera> m_Camera;
		std::unique_ptr<Shader> m_ModelShader;
		std::unique_ptr<ECS::Registry>m_Registry;
		std::unique_ptr<Game::RenderSystem>m_RenderSystem;
		std::unique_ptr<Game::InputSystem>m_InputSystem;
		std::unique_ptr<Game::CameraSystem>m_CameraSystem;
		bool m_Running = true;

		void InitRenderer();
		void DrawSquare(float x, float y, float size, float r, float g, float b);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
