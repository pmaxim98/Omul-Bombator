#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Config.hpp>
#include <SFML\Graphics\Color.hpp>
#include <SFML\Network\Packet.hpp>

#include <string>
#include <type_traits>

namespace Command
{
	namespace Movement 
	{
		enum OMUL_BOMBATOR_LIB_API : sf::Uint16
		{ 
			None = 0x00,
			Up = 0x01,
			Down = 0x02,
			Left = 0x04,
			Right= 0x08
		};
	}

	namespace Action 
	{
		enum OMUL_BOMBATOR_LIB_API : sf::Uint16
		{ 
			None = 0x00,
			Plant = 0x01, 
			Detonate = 0x02
		};
	}

	namespace Network 
	{
		enum OMUL_BOMBATOR_LIB_API : sf::Uint16
		{ 
			None = 0x00,
			Join = 0x01, 
			Leave = 0x02
		};
	}
	
	namespace Appearance 
	{
		enum OMUL_BOMBATOR_LIB_API : sf::Uint16
		{ 
			None = 0x00,
			Color = 0x01, 
			Nickname = 0x02
		};
	}

	struct OMUL_BOMBATOR_LIB_API CMD
	{
		CMD();

		sf::Uint16 movement;
		sf::Uint16 action;
		sf::Uint16 network;
		sf::Uint16 appearance;

		struct OMUL_BOMBATOR_LIB_API
		{
			std::string nickname;
			sf::Color color;
		} additionalData;

		bool empty() const;
	};
}

OMUL_BOMBATOR_LIB_API sf::Packet & operator <<(sf::Packet & packet, const Command::CMD & cmd);
OMUL_BOMBATOR_LIB_API sf::Packet & operator >>(sf::Packet & packet, Command::CMD & cmd);