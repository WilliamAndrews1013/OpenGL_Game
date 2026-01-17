#include <enet/enet.h>
#include <stdio.h>
#include<string.h>

int main(int argc, char** argv) 
{
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occured while initializing ENet. \n");
		return EXIT_FAILURE;
	}

	ENetHost* client = enet_host_create(NULL,	// Create a client host
		1,		// Only allow 1 outgoing connection
		2,		// Allow up to 2 channels to be used, 0 and 1
		0,		// Assume any amount of incoming bandwidth
		0);		// Assume any amount of outoging bandwidth

	if (client == NULL) {
		fprintf(stderr, "An error occured while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}

	ENetAddress address;
	ENetEvent event;
	ENetPeer* peer;

	// Connect to some.server.net:1234
	enet_address_set_host(&address, "some.server.net");
	address.port = 1234;

	// Inititate the connection, allocating the two channels 0 and 1
	peer = enet_host_connect(client, &address, 2, 0);

	if (peer == NULL) {
		fprintf(stderr, "No available peers for initiating an ENet connection.\n");
		exit(EXIT_FAILURE);
	}

	// Wait up to 5 seconds for the connection attempt to succeed
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		puts("Connection to some.server.net:1234 succeeded");
	}
	else {
		// Either 5 seconds are up or disconnect event was received. Reset the peer in the event 
		// the 5 seconds had run out without significant event
		enet_peer_reset(peer);
		puts("Connection to some.server.net:1234 failed.");
	}

	enet_host_destroy(client);
	enet_deinitialize();
	return 0;
}