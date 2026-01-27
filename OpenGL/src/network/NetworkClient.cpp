#include <network/NetworkClient.hh>
#include <network/Protocol.h>

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Winmm.lib")
#endif // _WIN32


namespace Network {
	NetworkClient::NetworkClient()
	{
		if (enet_initialize() != 0) {
			std::cerr << "An error occured while initializing ENet." << std::endl;
			return;
		}

		// Create client host
		m_ClientHost = enet_host_create(nullptr, 1, 2, 0, 0);
		if (m_ClientHost == nullptr) {
			std::cerr << "An error occured while trying to create an ENet client host." << std::endl;
		}
	}

	NetworkClient::~NetworkClient()
	{
		if (m_ServerPeer) {
			enet_peer_disconnect_now(m_ServerPeer, 0);
		}

		if (m_ClientHost) {
			enet_host_destroy(m_ClientHost);
		}

		enet_deinitialize();
	}

	bool NetworkClient::Connect(const std::string& ip, int port)
	{
		if (!m_ClientHost) return false;

		ENetAddress address;
		enet_address_set_host(&address, ip.c_str());
		address.port = port;

		// Initiate connection
		m_ServerPeer = enet_host_connect(m_ClientHost, &address, 2, 0);

		return (m_ServerPeer != nullptr);
	}

	void NetworkClient::Disconnect()
	{
		if (m_ServerPeer) {
			enet_peer_disconnect(m_ServerPeer, 0);
		}
	}

	void NetworkClient::Update(std::vector<Player>& playerList)
	{
		if (!m_ClientHost) return;

		ENetEvent event;

		// Timeout = 0 means NON-BLOCKING. It returns immediately if no packets
		while (enet_host_service(GetHost(), &event, 0) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				std::cout << "Connection Succeeded!" << std::endl;
				m_IsConnected = true;
				break;
			case ENET_EVENT_TYPE_RECEIVE: {
				printf("Server: ", event.packet->data);

				//TODO: Parse game data here (e.g., Player positions)

				// Check the first byte to see what kind of packet it is
				uint8_t* type = (uint8_t*)event.packet->data;

				if (*type == PACKET_SPAWN) {
					// Cast raw data to our struct
					PacketSpawn* packet = (PacketSpawn*)event.packet->data;

					// Add to our list
					Player newPlayer;
					newPlayer.id = packet->id;
					newPlayer.x = packet->x;
					newPlayer.y = packet->y;
					newPlayer.r = packet->r;
					newPlayer.g = packet->g;
					newPlayer.b = packet->b;

					// Directly add to the list
					playerList.push_back(newPlayer);

					printf("Spawned Player %d\n", newPlayer.id);
				}

				if (*type == PACKET_MOVE) {
					PacketMove* packet = (PacketMove*)event.packet->data;

					// Find the player in our list and update
					for (auto& player : playerList) {
						if (player.id == packet->id) {
							player.x = packet->x;
							player.y = packet->y;
							break;
						}
					}
				}

				enet_packet_destroy(event.packet);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
				std::cout << "Disconnected." << std::endl;
				m_IsConnected = false;
				m_ServerPeer = nullptr;
				break;
			}
		}
	}

	void NetworkClient::SendPacket(const std::string& message)
	{
		if (!m_IsConnected || !m_ServerPeer) return;

		// Create packet (Flag reliable ensures delivery)
		ENetPacket* packet = enet_packet_create(message.c_str(), message.length() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_ServerPeer, 0, packet);
	}

	void NetworkClient::SendMovePacket(float x, float y, float z)
	{
		if (!m_IsConnected || !m_ServerPeer) return;
		PacketMove movePacket;
		movePacket.type = PACKET_MOVE;
		movePacket.id = 0;
		movePacket.x = x;
		movePacket.y = y;
		movePacket.z = z;

		ENetPacket* packet = enet_packet_create(&movePacket, sizeof(PacketMove), ENET_PACKET_FLAG_UNSEQUENCED);
		printf("Sending Move: %f, %f\n", movePacket.x, movePacket.y);
		enet_peer_send(m_ServerPeer, 0, packet);
	}
}