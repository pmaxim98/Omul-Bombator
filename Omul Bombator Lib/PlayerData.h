#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Network\Packet.hpp>

#include "BomberData.h"
#include "PlayerStatus.h"
#include "PlayerTag.h"

struct OMUL_BOMBATOR_LIB_API PlayerData
{
	BomberData bomberData;
	PlayerStatus status;
	PlayerTag tag;
};

OMUL_BOMBATOR_LIB_API sf::Packet & operator <<(sf::Packet & packet, const PlayerData &);
OMUL_BOMBATOR_LIB_API sf::Packet & operator >>(sf::Packet & packet, PlayerData &);

OMUL_BOMBATOR_LIB_API bool operator==(const PlayerData & lhs, const PlayerData & rhs);
OMUL_BOMBATOR_LIB_API bool operator!=(const PlayerData & lhs, const PlayerData & rhs);