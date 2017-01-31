#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Network\Packet.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\Network\IpAddress.hpp>
#include <SFML\Graphics\Color.hpp>

#include "GameChanges.h"

OMUL_BOMBATOR_LIB_API sf::Packet & operator <<(sf::Packet & packet, const Change::World::Property & change);
OMUL_BOMBATOR_LIB_API sf::Packet & operator >>(sf::Packet & packet, Change::World::Property & change);

OMUL_BOMBATOR_LIB_API sf::Packet & operator <<(sf::Packet & packet, const sf::Color & color);
OMUL_BOMBATOR_LIB_API sf::Packet & operator >>(sf::Packet & packet, sf::Color & color);

template <typename T>
sf::Packet & operator <<(sf::Packet & packet, const sf::Vector2<T> & vec)
{
	return packet << vec.x << vec.y;
}

template <typename T>
sf::Packet & operator >>(sf::Packet & packet, sf::Vector2<T> & vec)
{
	return packet >> vec.x >> vec.y;
}

template <typename C, typename T>
sf::Packet & operator <<(sf::Packet & packet, const std::pair<C, T> & p)
{
	return packet << p.first << p.second;
}

template <typename C, typename T>
sf::Packet & operator >>(sf::Packet & packet, std::pair<C, T> & p)
{
	return packet >> p.first >> p.second;
}

template <typename C, typename T>
sf::Packet & operator <<(sf::Packet & packet, const std::vector<std::pair<C, T>> & vec)
{
	sf::Uint32 size = vec.size();
	packet << size;
	for (sf::Uint32 index = 0; index < size; ++index)
		packet << vec[index];
	return packet;
}

template <typename C, typename T>
sf::Packet & operator >>(sf::Packet & packet, std::vector<std::pair<C, T>> & vec)
{
	sf::Uint32 size;
	packet >> size;
	vec.resize(size);
	for (sf::Uint32 index = 0; index < size; ++index)
		packet >> vec[index];
	return packet;
}

template <typename T>
sf::Packet & operator <<(sf::Packet & packet, const std::vector<T> & vec)
{
	sf::Uint32 size = vec.size();
	packet << size;
	for (sf::Uint32 index = 0; index < size; ++index)
		packet << vec[index];
	return packet;
}

template <typename T>
sf::Packet & operator >>(sf::Packet & packet, std::vector<T> & vec)
{
	sf::Uint32 size;
	packet >> size;
	vec.resize(size);
	for (sf::Uint32 index = 0; index < size; ++index)
		packet >> vec[index];
	return packet;
}

template <typename T>
sf::Packet & operator <<(sf::Packet & packet, const std::vector<sf::Vector2<T>> & vec)
{
	sf::Uint32 size = vec.size();
	packet << size;
	for (sf::Uint32 index = 0; index < size; ++index)
		packet << vec[index];
	return packet;
}

template <typename T>
sf::Packet & operator >>(sf::Packet & packet, std::vector<sf::Vector2<T>> & vec)
{
	sf::Uint32 size;
	packet >> size;
	vec.resize(size);
	for (sf::Uint32 index = 0; index < size; ++index)
		packet >> vec[index];
	return packet;
}

