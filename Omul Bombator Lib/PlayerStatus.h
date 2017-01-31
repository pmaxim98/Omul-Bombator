#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Network\Packet.hpp>
#include <SFML\Config.hpp>
#include <string>

struct OMUL_BOMBATOR_LIB_API PlayerStatus
{
	sf::Int16 hp;
	sf::Int16 speed;

	sf::Int16 bombsDamage;
	sf::Int16 bombsRange;
	sf::Int16 bombsCarryLimit;

	sf::Int16 kills;
	sf::Int16 deaths;

	sf::Int16 damageReceived;
};

OMUL_BOMBATOR_LIB_API sf::Packet & operator <<(sf::Packet & packet, const PlayerStatus &);
OMUL_BOMBATOR_LIB_API sf::Packet & operator >>(sf::Packet & packet, PlayerStatus &);

OMUL_BOMBATOR_LIB_API bool operator==(const PlayerStatus & lhs, const PlayerStatus & rhs);
OMUL_BOMBATOR_LIB_API bool operator!=(const PlayerStatus & lhs, const PlayerStatus & rhs);