#include <enet/enet.h>
#include <stdio.h>

int main(int argc, char** argv) 
{
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occured while initializing ENet. \n");
		return EXIT_FAILURE;
	}

	ENetAddress address;
	ENetHost *server;

	/* Bind the server to the default localhost */
	/* A specific host address can be specified by */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;

	/* Bind the server to port 1234 */
	address.port = 1234;

	server = enet_host_create(&address,	// The address to bind the server host to
								32,		// Allow up to 32 clients and/or outgoing connections
								2,		// Allow up to 2 channels to be used, 0 and 1
								0,		// Assume any amount of incoming bandwidth
								0);		// Assume any amount of outgoing bandwidth

	if (server == NULL) {
		fprintf(stderr, "An error occured while trying to create an ENet server host.\n");
		exit(EXIT_FAILURE);
	}

	ENetEvent event;

	// Wait up to 1000 milliseconds for an event
	while (enet_host_service(server, &event, 1000) > 0) {
		switch (event.type) {
		case ENET_EVENT_TYPE_CONNECT:
			printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);

			// Store any relevant client information here
			event.peer->data = "Client Information";

			break;
		case ENET_EVENT_TYPE_RECEIVE:
			printf("A packet of length %u containing %s was recieved from %s on channel %u.\n", event.packet->dataLength, event.packet->data, event.peer->data, event.channelID);

			// Clean up packet now that we are done using it
			enet_packet_destroy(event.packet);

			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			printf("%s disconnected.\n", event.peer->data);

			// Reset the peer's client information
			event.peer->data = NULL;
		}
	}

	enet_host_destroy(server);
	enet_deinitialize();
}