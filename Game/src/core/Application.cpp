#include "core/Application.hh"
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "events/EventManager.hh"

namespace Core{
	Application::Application()
	{
		m_Window = std::make_unique<Window>(WindowProps("OpenGL Game Engine", 1024, 768));

		m_NetworkClient = std::make_unique<Network::NetworkClient>();
		if (m_NetworkClient->Connect("10.0.0.188", 3490)) {
			std::cout << "Connected successfully" << std::endl;
		}

		// Create camera at eye level for first-person view
		m_Camera = std::make_unique<Game::Camera>(glm::vec3(0.0f, 1.7f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		// Load model shader - path relative to build directory
		m_ModelShader = std::make_unique<Shader>("../assets/shaders/shader.vs", "../assets/shaders/shader.fs");

		m_Registry = std::make_unique<ECS::Registry>();
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent(entity, Game::TransformComponent{
			glm::vec3(0.0f, 0.0f, 0.0f),  // Position
			glm::vec3(0.0f, 0.0f, 0.0f),  // Rotation
			glm::vec3(1.0f, 1.0f, 1.0f)   // Scale
		});

		m_Registry->AddComponent(entity, Game::RenderableComponent{"../assets/model/toc_board.obj"});
		m_Registry->AddComponent(entity, Game::PlayerControllerComponent{});

		m_RenderSystem = std::make_unique<Game::RenderSystem>(m_ModelShader.get(), m_Camera.get());
		m_InputSystem = std::make_unique<Game::InputSystem>(m_Window.get());
		m_CameraSystem = std::make_unique<Game::CameraSystem>(m_Camera.get());
    	m_PlayerControllerSystem = std::make_unique<Game::PlayerControllerSystem>();
	}

	Application::~Application()
	{
		Events::gEventManager.Shutdown();

		// Cleanup ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::Run()
	{
		// Simple deltaTime calculation
		float lastFrameTime = 0.0f;

		while (m_Running) {

			float time = (float)glfwGetTime();
			float deltaTime = time - lastFrameTime;
			lastFrameTime = time;

			// Start imgui
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Define UI here
			ImGui::Begin("Game Debug");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::End();

			// Update ECS Systems (in order: Input -> Camera -> Render)
			m_InputSystem->Update(*m_Registry, deltaTime);
      		m_PlayerControllerSystem->Update(*m_Registry, deltaTime);
			m_CameraSystem->Update(*m_Registry, deltaTime);
			Events::gEventManager.DispatchEvents();
			m_RenderSystem->Update(*m_Registry, deltaTime);

			// Render ImGui over game
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Present and process events
			m_Window->OnUpdate();

			// Termination check
			if (m_Window->ShouldClose()) {
				m_NetworkClient->Disconnect();
				m_Running = false;
			}
		}
	}
}
