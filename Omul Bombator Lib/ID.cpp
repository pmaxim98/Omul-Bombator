#include "ID.h"


ID::ID()
	:
	value(0)
{
	
}

ID::ID(sf::Uint32 value)
	:
	value(value)
{

}

sf::Uint32 ID::get() const
{
	return value;
}

ID & ID::operator++()
{
	value++;
	return *this;
}

bool operator==(const ID & lhs, const ID & rhs)
{
	return lhs.get() == rhs.get();
}

bool operator!=(const ID & lhs, const ID & rhs)
{
	return !(lhs == rhs);
}

bool operator>=(const ID & lhs, const ID & rhs)
{
	return lhs.get() >= rhs.get();
}

bool operator>(const ID & lhs, const ID & rhs)
{
	return lhs.get() > rhs.get();
}

bool operator<=(const ID & lhs, const ID & rhs)
{
	return lhs.get() <= rhs.get();
}

bool operator<(const ID & lhs, const ID & rhs)
{
	return lhs.get() < rhs.get();
}

sf::Packet & operator<<(sf::Packet & packet, const ID & id)
{
	return packet << id.get();
}

sf::Packet & operator>>(sf::Packet & packet, ID & id)
{
	sf::Uint32 value;
	packet >> value;
	id = ID(value);
	return packet;
}

const ID ID::Invalid(0);
