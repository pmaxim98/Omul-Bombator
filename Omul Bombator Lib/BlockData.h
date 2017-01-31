#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Network\Packet.hpp>
#include <SFML\Config.hpp>
#include <SFML\System\Vector2.hpp>

#include "BlockType.h"

struct OMUL_BOMBATOR_LIB_API BlockData
{
	sf::Uint8 type;
	sf::Uint32 state;
	sf::Vector2<sf::Uint16> position;
};

OMUL_BOMBATOR_LIB_API sf::Packet & operator<<(sf::Packet & packet, const BlockData & data);
OMUL_BOMBATOR_LIB_API sf::Packet & operator>>(sf::Packet & packet, BlockData & data);

OMUL_BOMBATOR_LIB_API bool operator==(const BlockData & lhs, const BlockData & rhs);
OMUL_BOMBATOR_LIB_API bool operator!=(const BlockData & lhs, const BlockData & rhs);