#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Config.hpp>

enum OMUL_BOMBATOR_LIB_API BlockType : sf::Uint8
{
	Empty = 0x00,
	Explosion = 0x01,
	PowerUp = 0x02,
	Obstacle = 0x04,
	Wall = 0x08,
	Floor = 0x16
};