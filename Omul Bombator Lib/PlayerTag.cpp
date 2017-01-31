#include "PlayerTag.h"

sf::Packet & operator<<(sf::Packet & packet, const PlayerTag & tag)
{
	return packet << tag.id << tag.networkData << tag.nickname;
}

sf::Packet & operator >> (sf::Packet & packet, PlayerTag & tag)
{
	return packet >> tag.id >> tag.networkData >> tag.nickname;
}

bool operator==(const PlayerTag & lhs, const PlayerTag & rhs)
{
	return lhs.id == rhs.id && lhs.networkData == rhs.networkData && lhs.nickname == rhs.nickname;
}

bool operator!=(const PlayerTag & lhs, const PlayerTag & rhs)
{
	return !(lhs == rhs);
}
