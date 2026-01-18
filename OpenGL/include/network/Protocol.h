#pragma once
#include <stdint.h>

enum PacketType {
	PACKET_SPAWN = 0,
	PACKET_MOVE = 1
};

#pragma pack(push, 1)
typedef struct {
	uint8_t type;	// Uses PacketType enum
	uint32_t id;	// Unique ID for the player
	float x;
	float y;
	float r, g, b;	// Color
} PacketSpawn;
#pragma pack(pop)