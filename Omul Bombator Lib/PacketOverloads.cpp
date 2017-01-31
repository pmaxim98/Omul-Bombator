#include "PacketOverloads.h"

/*
sf::Packet & operator <<(sf::Packet & packet, const sf::Vector2f & vec)
{
	return packet << vec.x << vec.y;
}

sf::Packet & operator >>(sf::Packet & packet, sf::Vector2f & vec)
{
	return packet >> vec.x >> vec.y;
}

sf::Packet & operator <<(sf::Packet & packet, const std::vector<sf::Vector2f> & vec)
{
	for (const auto & v : vec) packet << v;
	return packet;
}

sf::Packet & operator >>(sf::Packet & packet, std::vector<sf::Vector2f> & vec)
{
	std::vector<sf::Vector2i> v;
	packet >> v;
	return packet >> vec;
}
*/

sf::Packet & operator<<(sf::Packet & packet, const Change::World::Property & change)
{
	return packet << static_cast<sf::Uint8>(change);
}

sf::Packet & operator>>(sf::Packet & packet, Change::World::Property & change)
{
	sf::Uint8 ch;
	packet >> ch;
	change = static_cast<Change::World::Property>(ch);
	return packet;
}

sf::Packet & operator<<(sf::Packet & packet, const sf::Color & color)
{
	return packet << color.toInteger();
}

sf::Packet & operator>>(sf::Packet & packet, sf::Color & color)
{
	sf::Uint32 colorInt;
	packet >> colorInt;
	color = sf::Color(colorInt);
	return packet;
}
