#include "game/LocalPlayer.hh"
#include <glm/glm.hpp>

namespace Game {
	LocalPlayer::LocalPlayer() : m_Position(0.0f, 0.0f, 0.0f), m_Speed(200.0f), m_LastSentX(0.0f), m_LastSentY(0.0f)
	{
	}
	void LocalPlayer::HandleInput(Core::Window* window, Network::NetworkClient* network, float deltaTime)
	{
		bool moved = false;

		// Process key presses
		if (window->IsKeyPressed(GLFW_KEY_W)) {
			m_Position.z -= m_Speed * deltaTime;
			moved = true;
		}
		if (window->IsKeyPressed(GLFW_KEY_S)) {
			m_Position.z += m_Speed * deltaTime;
			moved = true;
		}
		if (window->IsKeyPressed(GLFW_KEY_A)) {
			m_Position.x -= m_Speed * deltaTime;
			moved = true;
		}
		if (window->IsKeyPressed(GLFW_KEY_D)) {
			m_Position.x += m_Speed * deltaTime;
			moved = true;
		}

		// Only send if actually moved AND moved enough to matter
		if (moved) {
			if (glm::abs(m_Position.x - m_LastSentX) > 0.1f || glm::abs(m_Position.y - m_LastSentY) > 0.1f) {
				network->SendMovePacket(m_Position.x, m_Position.y, m_Position.z);
				m_LastSentX = m_Position.x;
				m_LastSentY = m_Position.y;
			}
		}
	}
}