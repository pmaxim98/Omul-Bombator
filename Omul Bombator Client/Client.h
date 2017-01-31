#pragma once

#include <SFML\Network\UdpSocket.hpp>

#include <WorldSnapshot.h>
#include <Command.h>

class Client
{
	public:
		Client() = delete;
		Client(const sf::IpAddress & serverip, const std::string & nickname, sf::Color bomberColor, WorldSnapshot & worldSnapshot);

		void sendCommands(const Command::CMD & cmd);
		bool receiveWorldSnapshot(WorldSnapshot & worldSnapshot);

	private:
		sf::UdpSocket socket;

		sf::IpAddress ownIP;
		unsigned short ownPort;

		sf::IpAddress serverIP;
		unsigned short serverPort;
};