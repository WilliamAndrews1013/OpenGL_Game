#pragma once

#include "core/Window.hh"
#include <memory>

namespace Core {

	class Application {
	public:
		
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}