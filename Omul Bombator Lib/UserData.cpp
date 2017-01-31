#include "UserData.h"

sf::Packet & operator<<(sf::Packet & packet, const UserData & userData)
{
	return packet << userData.status << userData.tag;
}

sf::Packet & operator >> (sf::Packet & packet, UserData & userData)
{
	return packet >> userData.status >> userData.tag;
}

bool operator==(const UserData & lhs, const UserData & rhs)
{
	return lhs.status == rhs.status && lhs.tag == rhs.tag;
}

bool operator!=(const UserData & lhs, const UserData & rhs)
{
	return !(lhs == rhs);
}
