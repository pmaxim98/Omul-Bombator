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

#include "States.h"

#include <string>

struct OMUL_BOMBATOR_LIB_API BomberData
{
	sf::Uint32 state;
	sf::Vector2f position;
	sf::Color color;
};

OMUL_BOMBATOR_LIB_API sf::Packet & operator<<(sf::Packet & packet, const BomberData & data);
OMUL_BOMBATOR_LIB_API sf::Packet & operator>>(sf::Packet & packet, BomberData & data);

OMUL_BOMBATOR_LIB_API bool operator==(const BomberData & lhs, const BomberData & rhs);
OMUL_BOMBATOR_LIB_API bool operator!=(const BomberData & lhs, const BomberData & rhs);