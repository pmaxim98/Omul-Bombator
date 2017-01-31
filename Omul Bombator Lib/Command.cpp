#include "Command.h"

#include "PacketOverloads.h"

sf::Packet & operator <<(sf::Packet & packet, const Command::CMD & cmd)
{
	return packet 
		<< cmd.action 
		<< cmd.movement
		<< cmd.appearance
		<< cmd.network
		<< cmd.additionalData.color
		<< cmd.additionalData.nickname;
}

sf::Packet & operator >>(sf::Packet & packet, Command::CMD & cmd)
{
	return packet 
		>> cmd.action 
		>> cmd.movement
		>> cmd.appearance
		>> cmd.network
		>> cmd.additionalData.color
		>> cmd.additionalData.nickname;
}

Command::CMD::CMD()
	:
	action(0),
	movement(0),
	appearance(0),
	network(0)
{
	additionalData.color = sf::Color::White;
	additionalData.nickname = "";
}

bool Command::CMD::empty() const
{
	return !(movement | action | appearance | network);
}
