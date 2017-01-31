#include "PlayerStatus.h"

sf::Packet & operator<<(sf::Packet & packet, const PlayerStatus & status)
{
	return packet << 
		status.bombsCarryLimit << 
		status.bombsRange << 
		status.bombsDamage << 
		status.deaths << 
		status.hp << 
		status.kills << 
		status.speed << 
		status.damageReceived;
}

sf::Packet & operator>>(sf::Packet & packet, PlayerStatus & status)
{
	return packet >> 
		status.bombsCarryLimit >> 
		status.bombsRange >> 
		status.bombsDamage >> 
		status.deaths >> 
		status.hp >> 
		status.kills >> 
		status.speed >>
		status.damageReceived;
}

bool operator==(const PlayerStatus & lhs, const PlayerStatus & rhs)
{
	return lhs.bombsCarryLimit == rhs.bombsCarryLimit 
		&& lhs.bombsRange == rhs.bombsRange
		&& lhs.bombsDamage == rhs.bombsDamage
		&& lhs.deaths == rhs.deaths 
		&& lhs.hp == rhs.hp 
		&& lhs.kills == rhs.kills 
		&& lhs.speed == rhs.speed
		&& lhs.damageReceived == rhs.damageReceived;
}

bool operator!=(const PlayerStatus & lhs, const PlayerStatus & rhs)
{
	return !(lhs == rhs);
}
