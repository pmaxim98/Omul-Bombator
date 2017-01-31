#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Config.hpp>

namespace Change
{
	namespace World
	{
		enum OMUL_BOMBATOR_LIB_API Property : sf::Uint8
		{
			None = 0x00,
			Players = 0x01,
			Bombers = 0x02,
			Bombs = 0x04,
			Blocks = 0x08
		};
	}

	namespace Player
	{
		enum OMUL_BOMBATOR_LIB_API Property : sf::Uint8
		{
			None = 0x00,
			Nickname = 0x01,
			HP = 0x02,
			Speed = 0x04,
			BombsCarryLimit = 0x08,
			Kills = 0x10,
			Deaths = 0x20
		};
	}

	namespace Bomber
	{
		enum OMUL_BOMBATOR_LIB_API Property : sf::Uint8
		{
			None = 0x00,
			ID = 0x01,
			Position = 0x02,
			Color = 0x04,
			SpriteFrame = 0x08
		};
	}

	namespace Bomb
	{
		enum OMUL_BOMBATOR_LIB_API Property : sf::Uint8
		{
			None = 0x00,
			ID = 0x01,
			Position = 0x02,
			Color = 0x04,
			SpriteFrame = 0x08,
			TicksUntilExplosion = 0x10,
			Damage = 0x20,
			Range = 0x40
		};
	}

	namespace Block
	{
		enum OMUL_BOMBATOR_LIB_API Property : sf::Uint8
		{
			None = 0x00,
			Type = 0x01,
			SpriteFrame = 0x02
		};
	}
}