#include "Client.h"

#include <spdlog/spdlog.h>
#include <SFML/Network/Packet.hpp>

#include <PacketOverloads.h>
#include <PHeader.h>

Client::Client(const sf::IpAddress & serverip, const std::string & nickname, sf::Color bomberColor, WorldSnapshot & worldSnapshot)
	:
	serverIP(serverip),
	serverPort(35123u)
{
	socket.setBlocking(false);

	if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
		spdlog::get("console") -> error("Couldn't bind the socket client to the port: {0}!", socket.getLocalPort());
		return;
	}

	ownIP = sf::IpAddress::getPublicAddress();
	ownPort = socket.getLocalPort();

	spdlog::get("console") -> info("Successfully bound the client to the port {0}!", ownPort);
	
	sf::Packet packet;
	Command::CMD cmd;

	cmd.network |= Command::Network::Join;
	cmd.additionalData.nickname = nickname;
	cmd.additionalData.color = bomberColor;

	packet << PHeader() << cmd;
	socket.send(packet, serverIP, serverPort);

	spdlog::get("console") -> info("Sent user data to the server with IP - {0} and Port - {1}.", serverIP.toString(), serverPort);

	sf::Socket::Status status = sf::Socket::Error;

	sf::Clock waitTime;

	spdlog::get("console") -> info("Waiting for the server to respond!");
	while (sf::Socket::Done != status) {
		sf::Packet joinPacket;
		status = socket.receive(joinPacket, serverIP, serverPort);
		if (status == sf::Socket::Done) {
			joinPacket >> worldSnapshot;
			break;
		}

		if (waitTime.getElapsedTime().asSeconds() > 5.0f) {
			spdlog::get("console") -> error("Couldn't receive world data from the server! Reason: Exceded the timeout limit of five seconds.");
			std::getchar();
			std::exit(EXIT_FAILURE);
		}
	}

	spdlog::get("console") -> info("Successfully received world data from the server!");
}

void Client::sendCommands(const Command::CMD & cmd)
{
	if (!cmd.empty()) {
		sf::Packet packet;
		packet << PHeader() << cmd;
		socket.send(packet, serverIP, serverPort);
	}
}

bool Client::receiveWorldSnapshot(WorldSnapshot & worldSnapshot)
{
	sf::Packet packet;
	sf::IpAddress server;
	unsigned short port;
	PHeader header;

	sf::Socket::Status status = socket.receive(packet, server, port);

	if (status == sf::Socket::Done) {
		packet >> header >> worldSnapshot;
		return true;
	}

	return false;
}