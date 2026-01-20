#pragma once
#include "core/Window.hh"
#include "network/NetworkClient.hh"

namespace Game {
	class LocalPlayer {
	public:
		LocalPlayer();

		void HandleInput(Core::Window* window, Network::NetworkClient* network, float deltaTime);

		float GetX() const { return m_X; }
		float GetY() const { return m_Y; }

	private:
		float m_X;
		float m_Y;
		float m_Speed;

		float m_LastSentX;
		float m_LastSentY;
	};
}