#include "PHeader.h"

#include <chrono>

PHeader::PHeader()
	:
	departureTimeStamp(0),
	arrivalTimeStamp(0)
{

}

sf::Packet & operator >>(sf::Packet & packet, PHeader & header)
{
	header.arrivalTimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return packet >> header.departureTimeStamp;
}

sf::Packet & operator <<(sf::Packet & packet, PHeader & header)
{
	header.departureTimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return packet << header.departureTimeStamp;
}