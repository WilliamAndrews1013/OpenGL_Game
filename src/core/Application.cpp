#include "Application.hpp"
#include <iostream>

namespace Core {

    Application::Application() {
        m_Window = std::make_unique<Window>(WindowProps("OpenGL Game Engine", 1024, 768));
    }

    Application::~Application() {
    }

    void Application::Run() {
        while (m_Running) {
            
            // Should be moved to an event callback or similar
            if (m_Window->ShouldClose())
                m_Running = false;
                
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Window->OnUpdate();
        }
    }

}
