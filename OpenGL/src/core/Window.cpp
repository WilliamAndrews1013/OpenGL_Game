#include "Core/Window.hh"
#include <iostream>


namespace Core {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
	}

	Window::Window(const WindowProps& props)
	{
		Init(props);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::OnUpdate()
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	bool Window::IsKeyPressed(int keycode) const
	{
		auto state = glfwGetKey(m_Window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	void Window::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		std::cout << "Creating window " << props.Title << " (" << props.Width << ", " << props.Height << ")" << std::endl;

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			if (!success) {
				std::cerr << "Could not be initialize GLFW!" << std::endl;
				return;
			}
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		if (!m_Window) {
			std::cerr << "Could not create GLFW window!" << std::endl;
			glfwTerminate();
			return;
		}

		// Make context current before loading GL functions
		glfwMakeContextCurrent(m_Window);

		// Set user pointer so callbacks can access WindowData
		glfwSetWindowUserPointer(m_Window, &m_Data);

		// Load OpenGL function pointers via GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize GLAD!" << std::endl;

			// Cleanup
			glfwDestroyWindow(m_Window);
			glfwTerminate();
			return;
		}

		
		// Set initial viewport
		glViewport(0, 0, (int)m_Data.Width, (int)m_Data.Height);

		// Optionally enable vsync
		glfwSwapInterval(1);

		// Framebuffer resize callback (uses stores user pointer)
		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			glViewport(0, 0, width, height);
		});
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		s_GLFWInitialized = false;
	}

	
}
