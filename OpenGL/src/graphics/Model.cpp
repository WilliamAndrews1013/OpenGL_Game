#include "graphics/Model.h"
#include <iostream>
#include <glad/glad.h>

// This defines the library implementation
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace Graphics {
	Model::Model()
	{
	}

	Model::~Model()
	{
		if (m_VAO) {
			glDeleteVertexArrays(1, &m_VAO);
		}

		if (m_VBO) {
			glDeleteBuffers(1, &m_VBO);
		}
	}

	bool Model::Load(const std::string& filePath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		// Parse the file
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str());

		if (!warn.empty()) std::cout << "Model Warning: " << warn << std::endl;
		if (!err.empty()) std::cerr << "Model Error: " << err << std::endl;
		if (!ret) return false;

		// Convert raw data into our vertex format
		std::vector<Vertex> vertices;

		// Loop over all "shapes"
		for (const auto& shape : shapes) {
			// Loop over all faces
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex;

				// Extract Position (x, y, z)
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				// Extract Normal (nx, ny, nz)
				if (index.normal_index >= 0) {
					vertex.normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					};
				}

				// Extract Texture Coords
				if (index.texcoord_index >= 0) {
					vertex.texCoords = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}

				vertices.push_back(vertex);
			}
		}

		m_VertexCount = (int)vertices.size();

		// Upload to GPU
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		// Position Attribute (Location 0)
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// Normal Attribute (Location 1) - We aren't using this in shader yet, but good to have
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		// TexCoord Attribute (Location 2)
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		glBindVertexArray(0);

		printf("Loaded Model: %s (%d verticies)\n", filePath.c_str(), m_VertexCount);
		return false;
	}

	void Model::Draw()
	{
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);
		glBindVertexArray(0);
	}
}