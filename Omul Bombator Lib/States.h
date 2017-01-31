#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Config.hpp>

namespace States 
{
	enum OMUL_BOMBATOR_LIB_API Bomber : sf::Uint32
	{
		Idling = 0x000,
		Up = 0x001,
		Down = 0x002,
		Left = 0x004,
		Right = 0x008,
		Planting = 0x010,
		Detonating = 0x020,
		Dying = 0x040,
		Spawning = 0x080,
		Dead = 0x100
	};

	enum OMUL_BOMBATOR_LIB_API Bomb : sf::Uint32
	{
		Frozen = 0x000,
		Ticking = 0x001,
		Exploding = 0x002,
		Exploded = 0x003,
		Planted = 0x004
	};

	enum OMUL_BOMBATOR_LIB_API Block : sf::Uint32
	{
		Empty,
		Static,
		Destructing,
		CenterExploding,
		LeftExploding,
		RightExploding,
		UpExploding,
		DownExploding,
		VerticalExploding,
		HorizontalExploding,
		FirePowerupGlowing,
		MegaFirePowerupGlowing,
		BombPowerupGlowing,
		SpeedPowerupGlowing
	};
}