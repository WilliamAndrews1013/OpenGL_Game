#pragma once

#include "core/Window.hh"
#include "network/NetworkClient.hh"
#include "network/Protocol.h"
#include <memory>
#include <vector>

namespace Core {

	class Application {
	public:
		
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Network::NetworkClient> m_NetworkClient;
		std::unique_ptr<Window> m_Window;
		std::vector<Player> m_Players;
		bool m_Running = true;
		unsigned int m_QuadVAO = 0;
		unsigned int m_QuadVBO = 0;
		unsigned int m_ShaderProgram = 0;

		void InitRenderer();
		void DrawSquare(float x, float y, float size, float r, float g, float b);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}