#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML\Network\Packet.hpp>
#include <SFML\Config.hpp>

class OMUL_BOMBATOR_LIB_API ID
{
	public:
		ID();
		ID(sf::Uint32 value);
		
		//ID & operator =(const ID &) = default;
		//ID & operator =(ID &&) = default;

		sf::Uint32 get() const;

		ID & operator++();

		static const ID Invalid;

	private:
		sf::Uint32 value;
};


OMUL_BOMBATOR_LIB_API bool operator==(const ID & lhs, const ID & rhs);
OMUL_BOMBATOR_LIB_API bool operator!=(const ID & lhs, const ID & rhs);
OMUL_BOMBATOR_LIB_API bool operator>=(const ID & lhs, const ID & rhs);
OMUL_BOMBATOR_LIB_API bool operator>(const ID & lhs, const ID & rhs);
OMUL_BOMBATOR_LIB_API bool operator<=(const ID & lhs, const ID & rhs);
OMUL_BOMBATOR_LIB_API bool operator<(const ID & lhs, const ID & rhs);

OMUL_BOMBATOR_LIB_API sf::Packet & operator<<(sf::Packet & packet, const ID & id);
OMUL_BOMBATOR_LIB_API sf::Packet & operator>>(sf::Packet & packet, ID & id);
