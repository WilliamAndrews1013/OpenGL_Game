#include "Core/Application.hh"
namespace Core{
	
	const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    
    uniform vec2 uOffset; // Position of the square
    uniform vec3 uColor;  // Color of the square

    void main() {
        // Simple conversion to Normalized Device Coordinates (-1.0 to 1.0)
        // This assumes a 1024x768 window. You should use a Projection Matrix ideally.
        float ndcX = (aPos.x + uOffset.x) / 1024.0 * 2.0 - 1.0;
        float ndcY = (aPos.y + uOffset.y) / 768.0 * 2.0 - 1.0; // Flipped Y depending on camera
        
        gl_Position = vec4(ndcX, ndcY, 0.0, 1.0);
    }
	)";

	const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec3 uColor;
    
    void main() {
        FragColor = vec4(uColor, 1.0);
    }
	)";

	Application::Application()
	{
		m_Window = std::make_unique<Window>(WindowProps("OpenGL Game Engine", 1024, 768));
		InitRenderer();

		m_NetworkClient = std::make_unique<Network::NetworkClient>();
		if (m_NetworkClient->Connect("10.0.0.188", 3490)) {
			std::cout << "Connected successfully" << std::endl;
		}

		m_LocalPlayer = std::make_unique<Game::LocalPlayer>();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		// Simple deltaTime calculation
		float lastFrameTime = 0.0f;

		while (m_Running) {

			float time = (float)glfwGetTime();
			float deltaTime = time - lastFrameTime;
			lastFrameTime = time;

			m_NetworkClient->Update(m_Players);

			m_LocalPlayer->HandleInput(m_Window.get(), m_NetworkClient.get(), deltaTime);

			// Render / clear
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// Draw Local Player
			DrawSquare(m_LocalPlayer->GetX(), m_LocalPlayer->GetY(), 50.0f, 0.0f, 1.0f, 0.0f);

			for (const auto& player : m_Players) {
				// Draw other players
				DrawSquare(player.x, player.y, 50.0f, player.r, player.g, player.b);
			}

			// Present and process events
			m_Window->OnUpdate();

			// Termination check
			if (m_Window->ShouldClose()) {
				m_NetworkClient->Disconnect();
				m_Running = false;
			}
		}
	}

	void Application::InitRenderer()
	{
		// Compile Shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		m_ShaderProgram = glCreateProgram();
		glAttachShader(m_ShaderProgram, vertexShader);
		glAttachShader(m_ShaderProgram, fragmentShader);
		glLinkProgram(m_ShaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Define square vertices (50px by 50px square)
		float vertices[] = {
			0.0f,  0.0f,  // Bottom-Left
			50.0f, 0.0f,  // Bottom-Right
			50.0f, 50.0f, // Top-Right

			50.0f, 50.0f, // Top-Right
			0.0f,  50.0f, // Top-Left
			0.0f,  0.0f   // Bottom-Left
		};

		// Setup buffers (VAO & VBO)
		glGenVertexArrays(1, &m_QuadVAO);
		glGenBuffers(1, &m_QuadVBO);

		glBindVertexArray(m_QuadVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Tell OpenGL how to read the aPos attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Application::DrawSquare(float x, float y, float size, float r, float g, float b)
	{
		glUseProgram(m_ShaderProgram);

		// Send the position and color to the shader
		int offsetLoc = glGetUniformLocation(m_ShaderProgram, "uOffset");
		glUniform2f(offsetLoc, x, y);

		int colorLoc = glGetUniformLocation(m_ShaderProgram, "uColor");
		glUniform3f(colorLoc, r, g, b);

		glBindVertexArray(m_QuadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

}
