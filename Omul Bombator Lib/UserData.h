#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Network\Packet.hpp>

#include "PlayerStatus.h"
#include "PlayerTag.h"

struct OMUL_BOMBATOR_LIB_API UserData
{
	PlayerStatus status;
	PlayerTag tag;
};

OMUL_BOMBATOR_LIB_API sf::Packet & operator <<(sf::Packet & packet, const UserData &);
OMUL_BOMBATOR_LIB_API sf::Packet & operator >>(sf::Packet & packet, UserData &);

OMUL_BOMBATOR_LIB_API bool operator==(const UserData & lhs, const UserData & rhs);
OMUL_BOMBATOR_LIB_API bool operator!=(const UserData & lhs, const UserData & rhs);