#include "Server.h"

#include <spdlog/spdlog.h>
#include <SFML/Network/Packet.hpp>

#include <algorithm>

#include <PHeader.h>
#include <Command.h>

Server::Server(unsigned short port)
{
	socket.setBlocking(false);

	if (socket.bind(port) != sf::Socket::Done) {
		spdlog::get("console") -> error("Couldn't bind the socket server to the port: {0}", port);
		return;
	}

	spdlog::get("console") -> info("Successfully bound the server's socket to the port {0}!", port);
}

std::vector<UserCommands> Server::receiveCommands()
{
	sf::Packet packet;
	sf::IpAddress userIp;
	unsigned short port;
	PHeader header;
	Command::CMD commands;

	std::vector<UserCommands> allUsersCommands;

	while (socket.receive(packet, userIp, port) == sf::Socket::Done) {
		packet >> header >> commands;
		
		UserCommands user;
		user.commands = commands;
		user.networkData.ipAddress = userIp;
		user.networkData.port = port;

		allUsersCommands.emplace_back(user);
	}

	return allUsersCommands;
}

void Server::sendWorldSnapshot(const WorldSnapshot & changedWorldState, 
							   const std::vector<PlayerData> & changedPlayersState, 
							   const std::vector<PlayerData> & playersData,
							   const std::vector<ID> & existingEntities)
{
	for (const auto & playerData : playersData) {
		WorldSnapshot worldSnapshot;

		worldSnapshot.existingEntities = existingEntities;

		if (changedWorldState.bombers != nullptr) {
			worldSnapshot.bombers = std::make_shared<BomberVector>(*changedWorldState.bombers);
			worldSnapshot.bombers -> erase(std::remove_if(worldSnapshot.bombers -> begin(), worldSnapshot.bombers -> end(), 
										[&playerData] (const std::pair<ID, BomberData> & bomberData) { return playerData.tag.id == bomberData.first; }),  
										   worldSnapshot.bombers -> end());
			if (worldSnapshot.bombers -> size() == 0)
				worldSnapshot.bombers.reset();
		}

		if (!changedPlayersState.empty()) {
			worldSnapshot.users = std::make_shared<UsersVector>();

			for (const auto & changedPlayer : changedPlayersState) {
				if (changedPlayer.tag.id != playerData.tag.id) {
					UserData userData;

					userData.status = changedPlayer.status;
					userData.tag = changedPlayer.tag;
					worldSnapshot.users -> emplace_back(changedPlayer.tag.id, userData);
				}
			}

			if (worldSnapshot.users -> size() == 0)
				worldSnapshot.users.reset();
		}

		if (changedWorldState.bombs != nullptr)
			worldSnapshot.bombs = std::make_shared<BombVector>(*changedWorldState.bombs);
		if (changedWorldState.blocks != nullptr)
			worldSnapshot.blocks = std::make_shared<BlockVector>(*changedWorldState.blocks);
		
		auto changePlayerIt = std::find_if(changedPlayersState.begin(), changedPlayersState.end(),
							   [&playerData] (const PlayerData & changedPlayerData) { return changedPlayerData.tag.id == playerData.tag.id; });

		if (changePlayerIt != changedPlayersState.end())
			worldSnapshot.player = std::make_shared<PlayerData>(*changePlayerIt);

		sf::Packet packet;
		packet << PHeader() << worldSnapshot;
		socket.send(packet, playerData.tag.networkData.ipAddress, playerData.tag.networkData.port);
	}
}

void Server::sendPacket(sf::Packet & packet, sf::IpAddress ip, unsigned short port)
{
	socket.send(packet, ip, port);
}
