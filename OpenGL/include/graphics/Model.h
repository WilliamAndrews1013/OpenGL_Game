#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Graphics {
	class Model {
	public:
		Model();
		~Model();

		// Reads .obj file and uploads it to the GPU
		bool Load(const std::string& filePath);

		// Draws the model using the currently active shader
		void Draw();

	private:
		struct Vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texCoords;
		};

		unsigned int m_VAO = 0;
		unsigned int m_VBO = 0;
		int m_VertexCount = 0;
	};
}