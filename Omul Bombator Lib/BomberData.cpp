#include "BomberData.h"

#include "PacketOverloads.h"

sf::Packet & operator<<(sf::Packet & packet, const BomberData & data)
{
	return packet << data.color << data.position << data.state;
}

sf::Packet & operator >> (sf::Packet & packet, BomberData & data)
{
	return packet >> data.color >> data.position >> data.state;
}

bool operator==(const BomberData & lhs, const BomberData & rhs)
{
	return lhs.color == rhs.color && lhs.position == rhs.position && lhs.state == rhs.state;
}

bool operator!=(const BomberData & lhs, const BomberData & rhs)
{
	return !(lhs == rhs);
}
