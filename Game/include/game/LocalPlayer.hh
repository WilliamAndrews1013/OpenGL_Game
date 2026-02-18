#pragma once
#include "core/Window.hh"
#include "network/NetworkClient.hh"
#include <glm/glm.hpp>

namespace Game {
	class LocalPlayer {
	public:
		LocalPlayer();

		void HandleInput(Core::Window* window, Network::NetworkClient* network, float deltaTime);

		float GetX() const { return m_Position.x; }
		float GetY() const { return m_Position.y; }
		float GetZ() const { return m_Position.z; }

		// Helper for Camera
		glm::vec3 GetPosition() const { return m_Position; }

	private:
		glm::vec3 m_Position;
		float m_Speed;

		float m_LastSentX;
		float m_LastSentY;
	};
}