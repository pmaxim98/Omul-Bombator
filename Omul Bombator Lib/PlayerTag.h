#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Network\Packet.hpp>
#include <SFML\Config.hpp>
#include <string>

#include "ID.h"
#include "UserNetworkData.h"

struct OMUL_BOMBATOR_LIB_API PlayerTag
{
	ID id;
	std::string nickname;
	UserNetworkData networkData;
};

OMUL_BOMBATOR_LIB_API sf::Packet & operator <<(sf::Packet & packet, const PlayerTag &);
OMUL_BOMBATOR_LIB_API sf::Packet & operator >>(sf::Packet & packet, PlayerTag &);

OMUL_BOMBATOR_LIB_API bool operator==(const PlayerTag & lhs, const PlayerTag & rhs);
OMUL_BOMBATOR_LIB_API bool operator!=(const PlayerTag & lhs, const PlayerTag & rhs);