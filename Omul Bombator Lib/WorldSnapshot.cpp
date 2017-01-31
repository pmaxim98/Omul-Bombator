#include "WorldSnapshot.h"

#include "PacketOverloads.h"

void WorldSnapshot::allocate()
{
	player = std::make_shared<PlayerData>();
	bombers = std::make_shared<std::vector<std::pair<ID, BomberData>>>();
	bombs = std::make_shared<std::vector<std::pair<ID, BombData>>>();
	blocks = std::make_shared<std::vector<std::pair<ID, BlockData>>>();
	users = std::make_shared<std::vector<std::pair<ID, UserData>>>();
}

WorldSnapshot::WorldSnapshot()
{

}

WorldSnapshot::~WorldSnapshot()
{

}

sf::Packet & operator<<(sf::Packet & packet, const WorldSnapshot & worldSnapshot)
{
	packet << worldSnapshot.existingEntities;
	if (worldSnapshot.player != nullptr)
		packet << "1Y" << *worldSnapshot.player.get();
	else
		packet << "1N";

	if (worldSnapshot.bombers != nullptr)
		packet << "2Y" << *worldSnapshot.bombers.get();
	else
		packet << "2N";

	if (worldSnapshot.bombs != nullptr)
		packet << "3Y" << *worldSnapshot.bombs.get();
	else
		packet << "3N";

	if (worldSnapshot.blocks != nullptr)
		packet << "4Y" << *worldSnapshot.blocks.get();
	else
		packet << "4N";

	if (worldSnapshot.users != nullptr)
		packet << "5Y" << *worldSnapshot.users.get();
	else
		packet << "5N";

	return packet;
}

sf::Packet & operator>>(sf::Packet & packet, WorldSnapshot & worldSnapshot)
{
	packet >> worldSnapshot.existingEntities;

	std::string valid;
	packet >> valid;
	PlayerData playerData;
	if (valid == "1Y") {
		packet >> playerData;
		worldSnapshot.player = std::make_shared<PlayerData>(playerData);
	}

	packet >> valid;
	std::vector<std::pair<ID, BomberData>> bombersData;
	if (valid == "2Y") {
		packet >> bombersData;
		worldSnapshot.bombers = std::make_shared<decltype(bombersData)>(bombersData);
	}
	
	packet >> valid;
	std::vector<std::pair<ID, BombData>> bombsData;
	if (valid == "3Y") {
		packet >> bombsData;
		worldSnapshot.bombs = std::make_shared<decltype(bombsData)>(bombsData);
	}

	packet >> valid;
	std::vector<std::pair<ID, BlockData>> blocksData;
	if (valid == "4Y") {
		packet >> blocksData;
		worldSnapshot.blocks = std::make_shared<decltype(blocksData)>(blocksData);
	}

	packet >> valid;
	std::vector<std::pair<ID, UserData>> usersData;
	if (valid == "5Y") {
		packet >> usersData;
		worldSnapshot.users = std::make_shared<decltype(usersData)>(usersData);
	}

	return packet;
}