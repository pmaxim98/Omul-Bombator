#include "UserNetworkData.h"

bool operator==(const UserNetworkData & lhs, const UserNetworkData & rhs)
{
	return lhs.ipAddress == rhs.ipAddress && lhs.port == rhs.port;
}

bool operator!=(const UserNetworkData & lhs, const UserNetworkData & rhs)
{
	return !(lhs == rhs);
}

sf::Packet & operator<<(sf::Packet & packet, const UserNetworkData & data)
{
	return packet << data.ipAddress.toInteger() << data.port;
}

sf::Packet & operator>>(sf::Packet & packet, UserNetworkData & data)
{
	sf::Uint32 ip;
	packet >> ip;
	data.ipAddress = sf::IpAddress(ip);
	packet >> data.port;
	return packet;
}
