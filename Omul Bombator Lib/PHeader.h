#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\System.hpp>
#include <SFML\Network\Packet.hpp>

struct OMUL_BOMBATOR_LIB_API PHeader
{
	PHeader();

	sf::Int64 departureTimeStamp;
	sf::Int64 arrivalTimeStamp;
};

OMUL_BOMBATOR_LIB_API sf::Packet & operator >>(sf::Packet & packet, PHeader & header);
OMUL_BOMBATOR_LIB_API sf::Packet & operator <<(sf::Packet & packet, PHeader & header);
