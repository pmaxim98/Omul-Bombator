#pragma once

#include <SFML/Network/UdpSocket.hpp>

#include <UserCommands.h>
#include <PlayerData.h>
#include <WorldSnapshot.h>
#include <ID.h>

class Server
{
	public:
		Server() = default;
		Server(unsigned short port);
		
		std::vector<UserCommands> receiveCommands();
		void sendWorldSnapshot(const WorldSnapshot & changedWorldState, 
							   const std::vector<PlayerData> & changedPlayersState, 
							   const std::vector<PlayerData> & playersData,
							   const std::vector<ID> & existingEntities);
		void sendPacket(sf::Packet & packet, sf::IpAddress ip, unsigned short port);

	private:
		sf::UdpSocket socket;
};