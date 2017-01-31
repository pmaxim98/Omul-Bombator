#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Network\Packet.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Color.hpp>
#include <SFML\Config.hpp>

#include "ID.h"

struct OMUL_BOMBATOR_LIB_API BombData
{
	ID owner;
	sf::Uint32 state;
	sf::Vector2f position;
	sf::Color color;
	sf::Int16 ticksUntilExplosion;
};

OMUL_BOMBATOR_LIB_API sf::Packet & operator<<(sf::Packet & packet, const BombData & data);
OMUL_BOMBATOR_LIB_API sf::Packet & operator>>(sf::Packet & packet, BombData & data);

OMUL_BOMBATOR_LIB_API bool operator==(const BombData & lhs, const BombData & rhs);
OMUL_BOMBATOR_LIB_API bool operator!=(const BombData & lhs, const BombData & rhs);