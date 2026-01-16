#include "Core/Application.hh"
namespace Core{
	
	Application::Application()
	{
		m_Window = std::make_unique<Window>(WindowProps("OpenGL Game Engine", 1024, 768));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running) {
			
			// Render / clear
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// Present and process events
			m_Window->OnUpdate();

			// Termination check
			if (m_Window->ShouldClose()) {
				m_Running = false;
			}
		}
	}

}
