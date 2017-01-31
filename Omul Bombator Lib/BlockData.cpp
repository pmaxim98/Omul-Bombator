#include "BlockData.h"

#include "PacketOverloads.h"

sf::Packet & operator<<(sf::Packet & packet, const BlockData & data)
{
	return packet << data.type << data.position << data.state;
}

sf::Packet & operator>>(sf::Packet & packet, BlockData & data)
{
	return packet >> data.type >> data.position >> data.state;
}

bool operator==(const BlockData & lhs, const BlockData & rhs)
{
	return lhs.position == rhs.position && lhs.type == rhs.type && lhs.state == rhs.state;
}

bool operator!=(const BlockData & lhs, const BlockData & rhs)
{
	return !(lhs == rhs);
}
