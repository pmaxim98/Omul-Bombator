#include "BombData.h"

#include "PacketOverloads.h"

sf::Packet & operator<<(sf::Packet & packet, const BombData & data)
{
	return packet << data.color << data.owner << data.position << data.ticksUntilExplosion << data.state;
}

sf::Packet & operator>>(sf::Packet & packet, BombData & data)
{
	return packet >> data.color >> data.owner >> data.position >> data.ticksUntilExplosion >> data.state;
}

bool operator==(const BombData & lhs, const BombData & rhs)
{
	return 
		lhs.color == rhs.color && 
		lhs.owner == rhs.owner && 
		lhs.position == rhs.position &&
		lhs.state == rhs.state &&
		lhs.ticksUntilExplosion == rhs.ticksUntilExplosion;
}

bool operator!=(const BombData & lhs, const BombData & rhs)
{
	return !(lhs == rhs);
}
