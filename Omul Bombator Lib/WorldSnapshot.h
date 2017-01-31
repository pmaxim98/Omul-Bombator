#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Network\Packet.hpp>

#include "ID.h"
#include "GameChanges.h"
#include "WorldSnapshotTypedefs.h"

struct OMUL_BOMBATOR_LIB_API WorldSnapshot
{
	std::vector<ID> existingEntities;

	PlayerPtr player;
	UsersPtr users;
	BombersPtr bombers;
	BombsPtr bombs;
	BlocksPtr blocks;

	void allocate();

	WorldSnapshot();
	~WorldSnapshot();
};

OMUL_BOMBATOR_LIB_API sf::Packet & operator <<(sf::Packet & packet, const WorldSnapshot & worldSnapshot);
OMUL_BOMBATOR_LIB_API sf::Packet & operator >>(sf::Packet & packet, WorldSnapshot & worldSnapshot);
