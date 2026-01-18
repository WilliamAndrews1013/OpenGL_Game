#pragma once
#include "network/Protocol.h"
#include "GameData.h"
#include <enet/enet.h>
#include <string>
#include <vector>
#include <iostream>

namespace Network {
	class NetworkClient {
	private:
		ENetHost* m_ClientHost = nullptr;
		ENetPeer* m_ServerPeer = nullptr;
		bool m_IsConnected = false;

	public:
		NetworkClient();
		~NetworkClient();

		bool Connect(const std::string& ip, int port);
		void Disconnect();

		// Call this once per frame in OpenGL Loop
		void Update(std::vector<Player>& playerList);

		void SendPacket(const std::string& message);

		bool IsConnected() const { return m_IsConnected; }
		ENetHost* GetHost() { return m_ClientHost; }
	};
}