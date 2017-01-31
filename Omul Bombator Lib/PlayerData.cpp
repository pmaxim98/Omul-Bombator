#include "PlayerData.h"

sf::Packet & operator<<(sf::Packet & packet, const PlayerData & data)
{
	return packet << data.bomberData << data.status << data.tag;
}

sf::Packet & operator>>(sf::Packet & packet, PlayerData & data)
{
	return packet >> data.bomberData >> data.status >> data.tag;
}

bool operator==(const PlayerData & lhs, const PlayerData & rhs)
{
	return lhs.bomberData == rhs.bomberData && lhs.status == rhs.status && lhs.tag == rhs.tag;
}

bool operator!=(const PlayerData & lhs, const PlayerData & rhs)
{
	return !(lhs == rhs);
}
