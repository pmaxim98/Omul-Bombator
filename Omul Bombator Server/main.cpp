#include <spdlog\spdlog.h>
#include <SFML\System\Clock.hpp>
#include <SFML\System\Thread.hpp>

#include <ID.h>
#include <Command.h>
#include <PlayerData.h>
#include <UserNetworkData.h>
#include <WorldSnapshot.h>
#include <MathUtils.h>

#include <PacketOverloads.h>

#include <array>
#include <chrono>
#include <type_traits>

#include "Server.h"

class Bomb
{
	public:
	Bomb(ID owner, const BomberData & bomberData, const std::function<std::vector<ID>(const sf::IntRect & bombCollider)> & onBomb)
	{
		data.ticksUntilExplosion = 100;
		data.color = bomberData.color;
		data.position = sf::Vector2f(static_cast<float>(static_cast<int>(bomberData.position.x) / 50 * 50 + 25), 
									 static_cast<float>(static_cast<int>(bomberData.position.y + 10) / 50 * 50 + 25));
		data.owner = owner;
		data.state = States::Bomb::Ticking;
		timeLeft = data.ticksUntilExplosion;

		collider = sf::IntRect(data.position.x - 25, data.position.y - 20, 50, 45);
		
		bombersOnBomb = onBomb(collider);
	}

	void setPosition(sf::Vector2f pos)
	{
		data.position = pos;
		collider = sf::IntRect(data.position.x - 25, data.position.y - 20, 50, 45);
	}

	ID getOwner() const
	{
		return data.owner;
	}

	const BombData & getData() const
	{
		return data;
	}

	void update()
	{
		if (data.state & States::Bomb::Ticking) {
			timeLeft--;

			if (timeLeft <= 0)
				data.state = States::Bomb::Exploding;
		}
	}

	sf::IntRect collider;
	BombData data;
	sf::Int16 timeLeft;
	bool inverse;
	std::vector<ID> bombersOnBomb;

	Bomb() { }
	private:
};

class Bomber
{
	public:
	Bomber()
	{
		data.state = States::Bomber::Spawning;
		timeLeft = 28;
		alive = true;
		bombsPlanted = 0;
	}

	Bomber(sf::Color color) : Bomber()
	{
		data.color = color;
	}

	void update()
	{
		if (data.state & States::Bomber::Dying) {
			timeLeft--;

			if (timeLeft <= 0)
				data.state = States::Bomber::Dead;
		}
	}

	void setPosition(sf::Vector2f pos)
	{
		data.position = pos;
		collider = sf::IntRect(data.position.x - 20, data.position.y + 10, 40, 20);
	}

	void moveDown(float dt, float speed)
	{
		data.position.y += dt * speed;
		collider = sf::IntRect(data.position.x - 20, data.position.y + 10, 40, 20);
	}

	void moveUp(float dt, float speed)
	{
		data.position.y -= dt * speed;
		collider = sf::IntRect(data.position.x - 20, data.position.y + 10, 40, 20);
	}

	void moveLeft(float dt, float speed)
	{
		data.position.x -= dt * speed;
		collider = sf::IntRect(data.position.x - 20, data.position.y + 10, 40, 20);
	}

	void moveRight(float dt, float speed)
	{
		data.position.x += dt * speed;
		collider = sf::IntRect(data.position.x - 20, data.position.y + 10, 40, 20);
	}

	const BomberData & getData() const
	{
		return data;
	}

	sf::IntRect collider;
	sf::Int16 timeLeft;
	bool alive;
	sf::Int16 bombsPlanted;
	BomberData data;

	private:
};

class Block
{
	public:
		Block()
		{
			owner = ID::Invalid;
			timeLeft = 10;
			data.state = States::Block::Static;
		}

		Block(BlockType type, sf::Vector2<sf::Uint16> tilePosition) : Block()
		{
			data.type = type;
			data.position = tilePosition;
			collider = sf::IntRect(data.position.y * 50, data.position.x * 50, 50, 50);
		}

		Block(BlockType type, sf::Vector2<sf::Uint16> tilePosition, States::Block state) : Block(type, tilePosition)
		{
			data.state = state;
		}

		Block(BlockType type, sf::Vector2<sf::Uint16> tilePosition, States::Block state, ID entityOwner) : Block(type, tilePosition, state)
		{
			owner = entityOwner;
		}

		void update()
		{
			if (data.state != States::Block::Static && data.type != BlockType::PowerUp) {
				if (data.state != States::Block::Empty) {
					timeLeft--;

					if (timeLeft <= 0) 
						data.state = States::Block::Empty;
				}
			}
		}

		const BlockData & getData() const
		{
			return data;
		}

		ID owner;
		sf::Int16 timeLeft;
		sf::IntRect collider;
		BlockData data;

	private:
};

class Player
{
	public:
	Player()
	{
		status.hp = 100;
		status.bombsRange = 1;
		status.bombsCarryLimit = 1;
		status.bombsDamage = 15;
		status.kills = 0;
		status.deaths = 0;
		status.speed = 150;
		status.damageReceived = 0;

		timeUntilSpawn = 150;
	}

	Player(ID id, const std::string & nickname, const UserNetworkData & networkData) : Player()
	{
		tag.id = id;
		tag.nickname = nickname;
		tag.networkData.port = networkData.port;
		tag.networkData.ipAddress = networkData.ipAddress;
	}

	void receiveDamage(sf::Int16 amount, Player & attacker)
	{
		if (status.hp > 0) {
			if (status.hp - amount <= 0) {
				timeUntilSpawn = 150;

				status.hp = 0;
				status.deaths++;
				status.damageReceived = amount;

				status.bombsCarryLimit /= 2;
				status.bombsCarryLimit = std::max<sf::Int16>(1, status.bombsCarryLimit);

				status.bombsRange /= 2;
				status.bombsRange = std::max<sf::Int16>(1, status.bombsRange);

				status.speed -= 20;
				status.speed = std::max<sf::Int16>(150, status.speed);

				status.bombsDamage -= 10;
				status.bombsDamage = std::max<sf::Int16>(15, status.bombsDamage);

				if (attacker.tag.id == tag.id) {
					status.kills--;
				}
				else {
					attacker.status.kills++;
					attacker.status.speed += 5;
					attacker.status.speed = std::min<sf::Int16>(230, attacker.status.speed);
					attacker.status.bombsDamage += 5;
					attacker.status.bombsDamage = std::min<sf::Int16>(50, attacker.status.bombsDamage);
				}
			}
			else {
				status.hp -= amount;
			}
		}
	}

	void receivePowerUp(sf::Uint32 powerupState)
	{
		if (powerupState == States::Block::FirePowerupGlowing)
			status.bombsRange = std::min<sf::Int16>(10, status.bombsRange + 1);
		else if (powerupState == States::Block::MegaFirePowerupGlowing)
			status.bombsRange = 10;
		else if (powerupState == States::Block::BombPowerupGlowing)
			status.bombsCarryLimit = std::min<sf::Int16>(10, status.bombsCarryLimit + 1);
		else if (powerupState == States::Block::SpeedPowerupGlowing)
			status.speed = std::min<sf::Int16>(230, status.speed + 10);
	}

	const UserNetworkData & getNetworkData() const
	{
		return tag.networkData;
	}

	const ID & getID() const
	{
		return tag.id;
	}

	const PlayerStatus & getStatus() const
	{
		return status;
	}

	const PlayerTag & getTag() const
	{
		return tag;
	}

	int timeUntilSpawn;
	PlayerStatus status;
	PlayerTag tag;
	private:
	
};

class IdsManager
{
	public:
	IdsManager()
		:
		currentID(0)
	{ }

	ID getNextID()
	{
		return ++currentID;
	}

	private:
	ID currentID;
};

struct EntitiesState
{
	std::map<ID, Bomber> bombers;
	std::map<ID, Bomb> bombs;
	std::map<ID, Block> blocks;
};

class World
{
	public:
	typedef std::vector<PlayerData> PlayersState;

	World()
	{
		std::string mapData =	
			"############"
			"#POOOOOOOOP#"
			"#O$O$OO$O$O#"
			"#OOOOOOOOOO#"
			"#O$O$PO$O$P#"
			"#P$O$OP$O$O#"
			"#OOOOOOOOOO#"
			"#O$O$OO$O$O#"
			"#POOOOOOOOP#"
			"############";

		for (sf::Uint16 pos = 0; pos < static_cast<sf::Uint16>(mapData.size()); ++pos) {
			if (mapData[pos] == '#')
				blocks.insert({idsManager.getNextID(), Block(BlockType::Wall, {pos / (Columns + 2u), pos % (Columns + 2u)})});
			else if (mapData[pos] == 'O')
				blocks.insert({idsManager.getNextID(), Block(BlockType::Floor, {pos / (Columns + 2u), pos % (Columns + 2u)})});
			else if (mapData[pos] == '$') {
				blocks.insert({idsManager.getNextID(), Block(BlockType::Obstacle, {pos / (Columns + 2u), pos % (Columns + 2u)})});
				blocks.insert({idsManager.getNextID(), Block(BlockType::Floor, {pos / (Columns + 2u), pos % (Columns + 2u)})});
			}
			else if (mapData[pos] == 'P') {
				blocks.insert({idsManager.getNextID(), Block(BlockType::Floor, {pos / (Columns + 2u), pos % (Columns + 2u)})});
				spawners.emplace_back(pos / (Columns + 2u), pos % (Columns + 2u));
			}
		}
	}

	std::vector<sf::Vector2<sf::Uint16>> spawners;

	void update(float dt, std::vector<UserCommands> userCmds, std::function<void(const WorldSnapshot &, sf::IpAddress, unsigned short)> sendWorldSnapshot)
	{
		for (const auto & user : userCmds)
		{
			if (user.commands.network & Command::Network::Join) {
				spdlog::get("console")->info("New player joined with nickname - {0} and IP - {1}:{2}.", user.commands.additionalData.nickname, user.networkData.ipAddress.toString(), user.networkData.port);

				ID playerID = createBomber(user.commands.additionalData.color);
				players.emplace_back(playerID, user.commands.additionalData.nickname, user.networkData);
				spawnBomber(playerID);

				spdlog::get("console")->info("Player: {0}, range: {1}", getPlayer(playerID).getID().get(), getPlayer(playerID).status.bombsRange);
		
				std::sort(players.begin(), players.end(), [] (const Player & lhs, const Player & rhs) { return lhs.getID() < rhs.getID(); });

				sendWorldSnapshot(generateWholeSnapshot(playerID), user.networkData.ipAddress, user.networkData.port);
				continue;
			}

			if (user.commands.network & Command::Network::Leave) {
				ID playerID = getPlayerId(user.networkData);

				if (playerID != ID::Invalid) {
					if (bombers.find(playerID) != bombers.end())
						bombers.erase(playerID);

					auto bombsID = getAllBombsByID(playerID);
					for (const auto bombID : bombsID)
						bombs.erase(bombID);

					removePlayer(playerID);
				}

				spdlog::get("console")->info("Player with nickname - {0} and IP - {1}:{2} left.", user.commands.additionalData.nickname, user.networkData.ipAddress.toString(), user.networkData.port);
				continue;
			}

			ID playerID = getPlayerId(user.networkData);

			if (!(bombers[playerID].data.state & States::Bomber::Dying || bombers[playerID].data.state & States::Bomber::Dead)) {
				bombers[playerID].data.state = States::Bomber::Idling;

				if (!(user.commands.movement & Command::Movement::None)) {
					float speed = getPlayer(playerID).getStatus().speed;

					if (user.commands.movement & Command::Movement::Down) {
						bombers[playerID].moveDown(dt, speed);
						bombers[playerID].data.state |= States::Bomber::Down;
					}
					if (user.commands.movement & Command::Movement::Up) {
						bombers[playerID].moveUp(dt, speed);
						bombers[playerID].data.state |= States::Bomber::Up;
					}
					if (user.commands.movement & Command::Movement::Left) {
						bombers[playerID].moveLeft(dt, speed);
						bombers[playerID].data.state |= States::Bomber::Left;
					}
					if (user.commands.movement & Command::Movement::Right) {
						bombers[playerID].moveRight(dt, speed);
						bombers[playerID].data.state |= States::Bomber::Right;
					}

					auto & bomberIt = bombers.find(playerID);
					handleCollisions(*bomberIt, user.commands.movement);
				}
				else {
					bombers[playerID].data.state &= ~(States::Bomber::Left | States::Bomber::Right | States::Bomber::Up | States::Bomber::Down);
				}

				if (!(user.commands.action & Command::Action::None)) {
					if (user.commands.action & Command::Action::Plant) {
						bombers[playerID].data.state |= States::Bomber::Planting;
						if (bombers[playerID].bombsPlanted < getPlayer(playerID).status.bombsCarryLimit)
							plantBomb(playerID);
					}
					if (user.commands.action & Command::Action::Detonate) {
						bombers[playerID].data.state |= States::Bomber::Detonating;
						detonateBombs(playerID);
					}
				}
				else {
					bombers[playerID].data.state &= ~(States::Bomber::Planting | States::Bomber::Detonating);
				}
			}
		}

		updateBombs();
		updateBlocks();
		updatePlayers();
	}

	void updatePlayers()
	{
		for (auto & player : players) {
			if (player.status.hp == 0) {
				if (player.timeUntilSpawn == 0) {
					spawnBomber(player.tag.id);
				}
				else {
					if (bombers[player.tag.id].alive) {
						bombers[player.tag.id].data.state = States::Bomber::Dying;
						bombers[player.tag.id].update();

						if (bombers[player.tag.id].data.state & States::Bomber::Dead) {
							bombers[player.tag.id].alive = false;
							bombers[player.tag.id].data.color = sf::Color(bombers[player.tag.id].data.color.r, bombers[player.tag.id].data.color.g, bombers[player.tag.id].data.color.b,  0);
							bombers[player.tag.id].data.position = sf::Vector2f(-1000, -1000);
						}
					}
					else {
						player.timeUntilSpawn--;
					}
				}
			}
		}
	}

	void updateBombs()
	{
		for (auto it = bombs.begin(); it != bombs.end();)
		{
			it->second.update();
			if (it->second.data.state == States::Bomb::Exploding) {
				explodeBomb(it->first);
			}
			if (it->second.data.state == States::Bomb::Exploded) {
				bombers[it->second.data.owner].bombsPlanted--;
				it = bombs.erase(it);
			}
			else
				++it;
		}
	}

	void updateBlocks()
	{
		createPowerUpRandomly();

		for (auto it = blocks.begin(); it != blocks.end();)
		{
			it->second.update();
			if (it->second.data.state == States::Block::Empty) {
				it = blocks.erase(it);
			}
			else
				++it;
		}
	}

	void spawnBomber(ID playerID)
	{
		auto randomSpawnPos = spawners[Math::genRandom(0, spawners.size() - 1)];

		bombers[playerID].setPosition(sf::Vector2f(static_cast<float>(randomSpawnPos.y * 50 + 25), static_cast<float>(randomSpawnPos.x * 50 + 25)));
		bombers[playerID].data.color = sf::Color(bombers[playerID].data.color.r, bombers[playerID].data.color.g, bombers[playerID].data.color.b, 255);
		bombers[playerID].data.state = States::Bomber::Spawning;
		bombers[playerID].timeLeft = 28;
		bombers[playerID].alive = true;

		getPlayer(playerID).status.hp = 100;
		getPlayer(playerID).timeUntilSpawn = 150;
	}

	void handleCollisions(std::pair<const ID, Bomber> & bomber, sf::Uint16 cmd)
	{
		auto computeNewPosition = [] (Bomber & bomber, sf::Uint16 cmd, const sf::IntRect & intersectionRect) {
			float horizontalDifference = 0.0f;
			float verticalDifference = 0.0f;

			if (cmd & Command::Movement::Right) {
				if (intersectionRect.height > intersectionRect.width) {
					horizontalDifference = -intersectionRect.width - 1;
					bomber.setPosition(sf::Vector2f(bomber.data.position.x + horizontalDifference, bomber.data.position.y));
					return;
				}
			}
			if (cmd & Command::Movement::Down) {
				if (intersectionRect.height < intersectionRect.width) {
					verticalDifference = -intersectionRect.height - 1;
					bomber.setPosition(sf::Vector2f(bomber.data.position.x, bomber.data.position.y + verticalDifference));
					return;
				}
			}
			if (cmd & Command::Movement::Left) {
				if (intersectionRect.height > intersectionRect.width) {
					horizontalDifference = intersectionRect.width + 1;
					bomber.setPosition(sf::Vector2f(bomber.data.position.x + horizontalDifference, bomber.data.position.y));
					return;
				}
			}
			if (cmd & Command::Movement::Up) {
				if (intersectionRect.height < intersectionRect.width) {
					verticalDifference = intersectionRect.height + 1;
					bomber.setPosition(sf::Vector2f(bomber.data.position.x, bomber.data.position.y + verticalDifference));
					return;
				}
			}
		};

		for (auto & block : blocks) {
			if (block.second.data.type == BlockType::Wall || block.second.data.type == BlockType::Obstacle) {
				sf::IntRect intersectionRect;

				if (bomber.second.collider.intersects(block.second.collider, intersectionRect))
					computeNewPosition(bomber.second, cmd, intersectionRect);
			}
			else if (block.second.data.type == BlockType::PowerUp) {
				if (bomber.second.collider.intersects(block.second.collider)) {
					getPlayer(bomber.first).receivePowerUp(block.second.data.state);
					block.second.data.state = States::Block::Empty;
				}	
			}
		}

		for (auto & bomb : bombs) {
			if (bomb.second.data.state == States::Bomb::Ticking) {
				auto onBombIt = std::find(bomb.second.bombersOnBomb.begin(), bomb.second.bombersOnBomb.end(), bomber.first);

				if (onBombIt != bomb.second.bombersOnBomb.end()) {
					if (!bomber.second.collider.intersects(bomb.second.collider))
						bomb.second.bombersOnBomb.erase(onBombIt);
				}
				else {
					sf::IntRect intersectionRect;
					if (bomber.second.collider.intersects(bomb.second.collider, intersectionRect))
						computeNewPosition(bomber.second, cmd, intersectionRect);
				}
			}
		}
	}

	ID plantBomb(ID planter)
	{
		const auto computeWhoStandsOnBomb = [this] (const sf::IntRect & bombCollider) -> std::vector<ID> {
			std::vector<ID> onBomb;
			for (const auto & bomber : bombers)
				if (bomber.second.collider.intersects(bombCollider))
					onBomb.emplace_back(bomber.first);
			return onBomb;
		};

		Bomb newBomb = Bomb(planter, bombers[planter].data, computeWhoStandsOnBomb);
		for (const auto & bomb : bombs)
			if (bomb.second.data.position == newBomb.data.position)
				return ID::Invalid;

		ID bombID = idsManager.getNextID();
		bombs.insert({bombID, newBomb});
		sf::Vector2<size_t> bombTile = getBombTilePosition(newBomb.data.position);
		activeBombs[bombTile.x][bombTile.y] = bombID.get();
		bombers[planter].bombsPlanted++;

		return bombID;
	}

	sf::Vector2<size_t> getBombTilePosition(sf::Vector2f position)
	{
		return sf::Vector2<size_t>(static_cast<size_t>(std::round(position.y)) / Tilesize - 1, 
								   static_cast<size_t>(std::round(position.x)) / Tilesize - 1);
	}

	void detonateBombs(ID detonater)
	{
		std::for_each(bombs.begin(), bombs.end(), [this, detonater] (std::pair<const ID, Bomb> & bomb) {
			if (bomb.second.data.owner == detonater) {
				explodeBomb(bomb.first);
			}
		});
	}

	enum Direction : sf::Uint16
	{
		None = 0x00,
		East = 0x01,
		West = 0x02,
		South = 0x04,
		North = 0x08
	};

	void createPowerUpRandomly()
	{
		auto pick = Math::genRandom(0, 5000);

		if (pick < 10) {
			if (std::count_if(blocks.begin(), blocks.end(), [] (const auto & blockPair) { return blockPair.second.data.type == BlockType::PowerUp; }) < 10 && 
				std::count_if(blocks.begin(), blocks.end(), [] (const auto & blockPair) { return blockPair.second.data.type == BlockType::Obstacle; }) < 8)
			{
				sf::Vector2<sf::Uint16> position = sf::Vector2<sf::Uint16>(Math::genRandom(1, 8), Math::genRandom(1, 10));
				ID blockId = ID::Invalid;
				sf::Uint8 tryCount = 10;

				while (tryCount && (activeBombs[position.x - 1][position.y - 1] 
									|| isBlockOfType(sf::Vector2i(position.x - 1, position.y - 1), BlockType::Obstacle, blockId)
									|| isBlockOfType(sf::Vector2i(position.x - 1, position.y - 1), BlockType::PowerUp, blockId))) 
				{
					position = sf::Vector2<sf::Uint16>(Math::genRandom(1, 8), Math::genRandom(1, 10));
					--tryCount;
				}

				if (tryCount == 0)
					return;

				spawnPowerUp(position, 100);
			}
		}
	}

	void spawnPowerUp(sf::Vector2<sf::Uint16> position, int chance)
	{
		auto pick = Math::genRandom(0, 100);

		if (pick <= chance) {
			int firstPowerUp = static_cast<int>(States::Block::FirePowerupGlowing);
			int lastPowerUp = static_cast<int>(States::Block::SpeedPowerupGlowing);
			int state = Math::genRandom(firstPowerUp, lastPowerUp);

			blocks.insert({idsManager.getNextID(), Block(BlockType::PowerUp, position, static_cast<States::Block>(state))});
		}
	}

	bool isBlockOfType(const sf::Vector2i & tilePos, sf::Uint8 blockType, ID & blockID) 
	{ 
		for (const auto & currentBlock : blocks) {
			if (currentBlock.second.data.type == blockType)
				if (currentBlock.second.data.position == sf::Vector2<sf::Uint16>(tilePos.x + 1, tilePos.y + 1)) {
					blockID = currentBlock.first;
					return true;
				}
		}
		blockID = ID::Invalid;
		return false;
	};

	void explodeBomb(ID bombID, sf::Uint16 source = None)
	{
		Bomb & bomb = bombs[bombID];
		
		if (bombs[bombID].data.state == States::Bomb::Exploded)
			return;

		bombs[bombID].data.state = States::Bomb::Exploded;
		const sf::Vector2<size_t> bombTile = getBombTilePosition(bomb.data.position);
		activeBombs[bombTile.x][bombTile.y] = 0;

		auto blockInsertionPair = blocks.insert({idsManager.getNextID(), Block(BlockType::Explosion, 
																	sf::Vector2<sf::Uint16>(bombTile.x + 1, bombTile.y + 1), 
																	States::Block::CenterExploding, 
																	bombID)});
		Block & currentBlock = blockInsertionPair.first->second;

		auto dealDamage = [this] (const Block & currentBlock) {
			for (auto & bomber : bombers)
				if (currentBlock.collider.intersects(bomber.second.collider)) {
					ID bombOwner = bombs[currentBlock.owner].data.owner;
					getPlayer(bomber.first).receiveDamage(getPlayer(bombOwner).status.bombsDamage, getPlayer(bombOwner));
				}
		};

		auto isExplosion = [this] (const sf::Vector2i & tilePos) -> bool { 
			for (const auto & currentBlock : blocks) {
				if (currentBlock.second.data.type == BlockType::Explosion && currentBlock.second.data.state == States::Block::CenterExploding)
					if (currentBlock.second.data.position == sf::Vector2<sf::Uint16>(tilePos.x + 1, tilePos.y + 1))
						return true;
			}
			return false;
		};

		dealDamage(currentBlock);

		int range = getPlayer(bombs[bombID].data.owner).status.bombsRange;
		int chanceOfPowerup = 20;

		if (!(source & Direction::East)) {
			for (int distance = 1; distance <= range; ++distance) {
				sf::Vector2i checkedPosition = sf::Vector2i(static_cast<int>(bombTile.x), static_cast<int>(bombTile.y) + distance);

				ID blockID = ID::Invalid;
				if (checkedPosition.x >= 0 
					&& checkedPosition.y >= 0 
					&& checkedPosition.x < Rows 
					&& checkedPosition.y < Columns 
					&& !isBlockOfType(checkedPosition, BlockType::Wall, blockID)) 
				{	
					if (!isBlockOfType(checkedPosition, BlockType::Obstacle, blockID)) {
						if (activeBombs[checkedPosition.x][checkedPosition.y]) {
							explodeBomb(activeBombs[checkedPosition.x][checkedPosition.y], Direction::West);
							break;
						}
						else {
							if (isBlockOfType(checkedPosition, BlockType::PowerUp, blockID))
								blocks[blockID].data.state = States::Block::Empty;

							if (isExplosion(checkedPosition))
								continue;

							decltype(blockInsertionPair) blockPair;

							if (distance == range)
								blockPair = blocks.insert({idsManager.getNextID(), Block(BlockType::Explosion, sf::Vector2<sf::Uint16>(checkedPosition.x + 1, checkedPosition.y + 1), States::Block::RightExploding, bombID)});
							else
								blockPair = blocks.insert({idsManager.getNextID(), Block(BlockType::Explosion, sf::Vector2<sf::Uint16>(checkedPosition.x + 1, checkedPosition.y + 1), States::Block::HorizontalExploding, bombID)});
							dealDamage(blockPair.first->second);
						}
					}
					else {
						blocks[blockID].data.state = States::Block::Destructing;
						spawnPowerUp(sf::Vector2<sf::Uint16>(checkedPosition.x + 1, checkedPosition.y + 1), chanceOfPowerup);
						break;
					}
				}
				else {
					break;
				}
			}
		}

		if (!(source & Direction::West)) {
			for (int distance = 1; distance <= range; ++distance) {
				sf::Vector2i checkedPosition = sf::Vector2i(static_cast<int>(bombTile.x), static_cast<int>(bombTile.y) - distance);

				ID blockID = ID::Invalid;
				if (checkedPosition.x >= 0 && checkedPosition.y >= 0 && checkedPosition.x < Rows && checkedPosition.y < Columns && !isBlockOfType(checkedPosition, BlockType::Wall, blockID)) {
					if (!isBlockOfType(checkedPosition, BlockType::Obstacle, blockID)) {
						if (activeBombs[checkedPosition.x][checkedPosition.y]) {
							explodeBomb(activeBombs[checkedPosition.x][checkedPosition.y], Direction::East);
							break;
						}
						else {
							if (isBlockOfType(checkedPosition, BlockType::PowerUp, blockID))
								blocks[blockID].data.state = States::Block::Empty;

							if (isExplosion(checkedPosition))
								continue;

							decltype(blockInsertionPair) blockPair;

							if (distance == range)
								blockPair = blocks.insert({idsManager.getNextID(), Block(BlockType::Explosion, sf::Vector2<sf::Uint16>(checkedPosition.x + 1, checkedPosition.y + 1), States::Block::LeftExploding, bombID)});
							else
								blockPair = blocks.insert({idsManager.getNextID(), Block(BlockType::Explosion, sf::Vector2<sf::Uint16>(checkedPosition.x + 1, checkedPosition.y + 1), States::Block::HorizontalExploding, bombID)});
							dealDamage(blockPair.first->second);
						}
					}
					else {
						blocks[blockID].data.state = States::Block::Destructing;
						spawnPowerUp(sf::Vector2<sf::Uint16>(checkedPosition.x + 1, checkedPosition.y + 1), chanceOfPowerup);
						break;
					}
				}
				else {
					break;
				}
			}
		}

		if (!(source & Direction::North)) {
			for (int distance = 1; distance <= range; ++distance) {
				sf::Vector2i checkedPosition = sf::Vector2i(static_cast<int>(bombTile.x) - distance, static_cast<int>(bombTile.y));

				ID blockID = ID::Invalid;
				if (checkedPosition.x >= 0 && checkedPosition.y >= 0 && checkedPosition.x < Rows && checkedPosition.y < Columns && !isBlockOfType(checkedPosition, BlockType::Wall, blockID)) {
					if (!isBlockOfType(checkedPosition, BlockType::Obstacle, blockID)) {
						if (activeBombs[checkedPosition.x][checkedPosition.y]) {
							explodeBomb(activeBombs[checkedPosition.x][checkedPosition.y], Direction::South);
							break;
						}
						else {
							if (isBlockOfType(checkedPosition, BlockType::PowerUp, blockID))
								blocks[blockID].data.state = States::Block::Empty;

							if (isExplosion(checkedPosition))
								continue;

							decltype(blockInsertionPair) blockPair;

							if (distance == range)
								blockPair = blocks.insert({idsManager.getNextID(), Block(BlockType::Explosion, sf::Vector2<sf::Uint16>(checkedPosition.x + 1, checkedPosition.y + 1), States::Block::UpExploding, bombID)});
							else
								blockPair = blocks.insert({idsManager.getNextID(), Block(BlockType::Explosion, sf::Vector2<sf::Uint16>(checkedPosition.x + 1, checkedPosition.y + 1), States::Block::VerticalExploding, bombID)});
							dealDamage(blockPair.first->second);
						}
					}
					else {
						blocks[blockID].data.state = States::Block::Destructing;
						spawnPowerUp(sf::Vector2<sf::Uint16>(checkedPosition.x + 1, checkedPosition.y + 1), chanceOfPowerup);
						break;
					}
				}
				else {
					break;
				}
			}
		}

		if (!(source & Direction::South)) {
			for (int distance = 1; distance <= range; ++distance) {
				sf::Vector2i checkedPosition = sf::Vector2i(static_cast<int>(bombTile.x) + distance, static_cast<int>(bombTile.y));

				ID blockID = ID::Invalid;
				if (checkedPosition.x >= 0 && checkedPosition.y >= 0 && checkedPosition.x < Rows && checkedPosition.y < Columns && !isBlockOfType(checkedPosition, BlockType::Wall, blockID)) {
					if (!isBlockOfType(checkedPosition, BlockType::Obstacle, blockID)) {
						if (activeBombs[checkedPosition.x][checkedPosition.y]) {
							explodeBomb(activeBombs[checkedPosition.x][checkedPosition.y], Direction::North);
							break;
						}
						else {
							if (isBlockOfType(checkedPosition, BlockType::PowerUp, blockID))
								blocks[blockID].data.state = States::Block::Empty;

							if (isExplosion(checkedPosition))
								continue;

							decltype(blockInsertionPair) blockPair;

							if (distance == range)
								blockPair = blocks.insert({idsManager.getNextID(), Block(BlockType::Explosion, sf::Vector2<sf::Uint16>(checkedPosition.x + 1, checkedPosition.y + 1), States::Block::DownExploding, bombID)});
							else
								blockPair = blocks.insert({idsManager.getNextID(), Block(BlockType::Explosion, sf::Vector2<sf::Uint16>(checkedPosition.x + 1, checkedPosition.y + 1), States::Block::VerticalExploding, bombID)});
							dealDamage(blockPair.first->second);
						}
					}
					else {
						blocks[blockID].data.state = States::Block::Destructing;
						spawnPowerUp(sf::Vector2<sf::Uint16>(checkedPosition.x + 1, checkedPosition.y + 1), chanceOfPowerup);
						break;
					}
				}
				else {
					break;
				}
			}
		}
	}

	std::vector<ID> getExistingEntities()
	{
		std::vector<ID> entities;

		auto addIds = [&entities] (const auto & vec) {
			std::for_each(std::begin(vec), std::end(vec), [&entities] (const auto & elem) { entities.emplace_back(elem.first); });
		};

		std::for_each(std::begin(players), std::end(players), [&entities] (const auto & elem) { entities.emplace_back(elem.getID()); });
		addIds(bombers);
		addIds(bombs);
		addIds(blocks);

		return entities;
	}

	std::vector<PlayerData> getPlayersData()
	{
		std::vector<PlayerData> data;

		for (const auto & player : players) {
			PlayerData playerData;
			playerData.tag = player.getTag();
			playerData.status = player.getStatus();
			playerData.bomberData = bombers[playerData.tag.id].getData();

			data.emplace_back(playerData);
		}

		return data;
	}

	template <typename C, typename M, typename P>
	void addChangedEntities(const M & nowEntities, const M & oldEntities, P & changeEntities)
	{
		for (const auto & nowEntityData : nowEntities) {
			const auto oldEntitiyDataIt = oldEntities.find(nowEntityData.first);

			if (oldEntitiyDataIt != oldEntities.end()) {
				if (oldEntitiyDataIt->second.getData() != nowEntityData.second.getData()) {
					if (changeEntities == nullptr)
						changeEntities = std::make_shared<C>();
					changeEntities->emplace_back(nowEntityData.first, nowEntityData.second.getData());
				}
			}
			else {

				if (changeEntities == nullptr)
					changeEntities = std::make_shared<C>();
				changeEntities->emplace_back(nowEntityData.first, nowEntityData.second.getData());
			}
		}
	}

	WorldSnapshot generateEntitiesSnap()
	{
		WorldSnapshot changeSnapshot;

		addChangedEntities<BombVector>(bombs, lastEntitiesState.bombs, changeSnapshot.bombs);
		addChangedEntities<BomberVector>(bombers, lastEntitiesState.bombers, changeSnapshot.bombers);
		addChangedEntities<BlockVector>(blocks, lastEntitiesState.blocks, changeSnapshot.blocks);

		lastEntitiesState.bombs = bombs;
		lastEntitiesState.bombers = bombers;
		lastEntitiesState.blocks = blocks;

		return changeSnapshot;
	}

	PlayersState generatePlayersSnap()
	{
		PlayersState changeSnapshot;

		const auto playersData = getPlayersData();

		for (const auto & nowPlayerData : playersData) {
			auto oldPlayerDataIt = std::find_if(lastPlayersState.begin(), lastPlayersState.end(),
												[&nowPlayerData] (const PlayerData & oldPlayerData) { return oldPlayerData.tag.id == nowPlayerData.tag.id; });

			if (oldPlayerDataIt != lastPlayersState.end()) {
				if (oldPlayerDataIt->status != nowPlayerData.status ||
					oldPlayerDataIt->tag != nowPlayerData.tag ||
					oldPlayerDataIt->bomberData != nowPlayerData.bomberData)
				{
					changeSnapshot.emplace_back(nowPlayerData);
				}
			}
			else {
				changeSnapshot.emplace_back(nowPlayerData);
			}
		}

		lastPlayersState = playersData;
		return changeSnapshot;
	}

	private:
	enum Properties : sf::Uint16
	{
		Rows = 8,
		Columns = 10,
		Tilesize = 50
	};

	std::map<ID, Bomber> bombers;
	std::map<ID, Bomb> bombs;
	std::map<ID, Block> blocks;

	std::vector<Player> players;

	std::array<std::array<sf::Uint64, Columns>, Rows> activeBombs;

	IdsManager idsManager;

	EntitiesState lastEntitiesState;
	PlayersState lastPlayersState;

	WorldSnapshot generateWholeSnapshot(ID playerID)
	{
		WorldSnapshot snapshot;

		snapshot.allocate();

		snapshot.player->status = getPlayer(playerID).getStatus();
		snapshot.player->tag = getPlayer(playerID).getTag();
		snapshot.player->bomberData = bombers[playerID].getData();

		for (const auto & bomber : bombers)
			if (bomber.first != playerID)
				snapshot.bombers->emplace_back(bomber.first, bomber.second.getData());

		for (const auto & bomb : bombs)
			snapshot.bombs->emplace_back(bomb.first, bomb.second.getData());

		for (const auto & block : blocks)
			snapshot.blocks->emplace_back(block.first, block.second.getData());

		for (const auto & player : players) {
			if (player.tag.id != playerID) {
				UserData userData;

				userData.status = player.status;
				userData.tag = player.tag;
				snapshot.users->emplace_back(player.tag.id, userData);
			}
		}

		return snapshot;
	}

	ID createBomber(sf::Color color)
	{
		ID id = idsManager.getNextID();
		bombers.insert({id, Bomber(color)});
		return id;
	}

	void createBomber(ID playerID)
	{
		bombers.insert({playerID, Bomber()});
	}

	// playerID = bomberID
	ID getPlayerId(const UserNetworkData & networkData) const
	{
		auto it = std::find_if(players.begin(), players.end(), [&networkData] (const Player & player) { return player.getNetworkData() == networkData; });

		if (it != players.end())
			return it->getID();
		return ID::Invalid;
	}

	const Player & getPlayer(ID id) const
	{
		auto it = std::find_if(players.begin(), players.end(), [&id] (const Player & player) { return player.getID() == id; });

		if (it != players.end())
			return *it;
		return *players.begin();
	}

	Player & getPlayer(ID id)
	{
		auto it = std::find_if(players.begin(), players.end(), [&id] (const Player & player) { return player.getID() == id; });

		if (it != players.end())
			return *it;
		return *players.begin();
	}

	void removePlayer(ID id)
	{
		players.erase(std::remove_if(players.begin(), players.end(), [&id] (const Player & player) { return player.getID() == id; }), players.end());
	}

	std::vector<ID> getAllBombsByID(ID bomberID) const
	{
		std::vector<ID> bombsIds;

		for (const auto & bomb : bombs)
			if (bomb.second.getOwner() == bomberID)
				bombsIds.emplace_back(bomb.first);

		return bombsIds;
	}
};

void network()
{
	World world;

	Server server(35123u);
	sf::Clock serverClock;

	auto sendWorldSnapshot = [&server] (const WorldSnapshot & worldSnapshot, sf::IpAddress userIp, unsigned short userPort) -> void
	{
		sf::Packet packet;
		packet << worldSnapshot;
		server.sendPacket(packet, userIp, userPort);
	};

	sf::Time timeSinceLastWorldUpdate = sf::Time::Zero;
	sf::Time timeSinceLastSnapshotSending = sf::Time::Zero;
	sf::Time timeAccumulated = sf::Time::Zero;

	sf::Uint64 currentTick = 0;
	const float tickRate = 30.0f; //200.0f / 3.0f;
	const float snapshotRate = 20.0f;
	const sf::Time timeStep = sf::seconds(1.0f / tickRate);
	const sf::Time snapshotStep = sf::seconds(1.0f / snapshotRate);

	sf::Clock worldTimer;
	sf::Clock snapShotTimer;

	while (true)
	{
		//spdlog::get("console") -> error("Tick: {0}.", currentTick);

		timeSinceLastWorldUpdate += timeAccumulated;

		while (timeSinceLastWorldUpdate >= timeStep) {
			//spdlog::get("console") -> info("World!");
			timeSinceLastWorldUpdate -= timeStep;

			world.update(timeStep.asSeconds(), server.receiveCommands(), sendWorldSnapshot);
		}

		timeAccumulated = worldTimer.restart();

		timeSinceLastSnapshotSending += snapShotTimer.restart();

		while (timeSinceLastSnapshotSending >= snapshotStep) {
			//spdlog::get("console") -> info("Snapshot!");
			timeSinceLastSnapshotSending -= snapshotStep;

			server.sendWorldSnapshot(world.generateEntitiesSnap(),
									 world.generatePlayersSnap(),
									 world.getPlayersData(),
									 world.getExistingEntities());
		}

		if (timeStep >= timeSinceLastWorldUpdate) {
			std::chrono::milliseconds timeUntilNextFrame(sf::Time(timeStep - timeSinceLastWorldUpdate).asMilliseconds());
			std::this_thread::sleep_for(timeUntilNextFrame);
		}

		currentTick++;
	}
}

int main()
{
	auto console = spdlog::stdout_color_mt("console");

	sf::Thread thread(std::bind(&network));
	thread.launch();

	return 0;
}