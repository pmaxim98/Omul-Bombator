#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Network\Packet.hpp>
#include <SFML\Network\IpAddress.hpp>

struct OMUL_BOMBATOR_LIB_API UserNetworkData
{
	unsigned short port;
	sf::IpAddress ipAddress;
};

OMUL_BOMBATOR_LIB_API bool operator==(const UserNetworkData & lhs, const UserNetworkData & rhs);
OMUL_BOMBATOR_LIB_API bool operator!=(const UserNetworkData & lhs, const UserNetworkData & rhs);

OMUL_BOMBATOR_LIB_API sf::Packet & operator<<(sf::Packet & packet, const UserNetworkData & data);
OMUL_BOMBATOR_LIB_API sf::Packet & operator>>(sf::Packet & packet, UserNetworkData & data);
