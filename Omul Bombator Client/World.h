#pragma once

#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>

#include <Command.h>
#include <WorldSnapshot.h>
#include <MathUtils.h>

#include "Bomb.h"
#include "Bomber.h"
#include "Block.h"
#include "Player.h"

class World : public sf::Drawable
{
	public:
		std::vector<sf::Vector2f> posSnapshots;

		void update(float delta, sf::Uint64 tick, const Command::CMD & cmd)
		{
			performPlayerUpdates(delta, cmd);
			performInterpolation(tick);
			performPrediction();
		}

		void performPrediction()
		{
			if (worldSnapshots.size() > 1) {
				const auto nowSnapIt = std::prev(worldSnapshots.end());
				const auto prevSnapIt = std::prev(nowSnapIt);

				if (nowSnapIt->second.player != prevSnapIt->second.player) {
					player.setPosition(nowSnapIt->second.player->bomberData.position);

					auto oldPos = std::find_if(std::begin(posSnapshots), std::end(posSnapshots), [this] (const auto & pos, const float eps = 0.0001f)
					{ return std::abs(player.getBomber().getPosition().x - pos.x) <= eps && std::abs(player.getBomber().getPosition().y - pos.y) <= eps; });

					if (oldPos != posSnapshots.end()) {
						auto remainIt = posSnapshots.erase(std::begin(posSnapshots), std::next(oldPos));

						while (remainIt != posSnapshots.end()) {
							player.setPosition(*remainIt);
							++remainIt;
						}
					}
				}
			}
		}

		void performPlayerUpdates(float delta, const Command::CMD & cmd)
		{
			if (worldSnapshots.size() > 1) {
				const auto nowSnapIt = std::prev(worldSnapshots.end());
				const auto prevSnapIt = std::prev(nowSnapIt);
				player.tag = prevSnapIt->second.player->tag;
				player.status = prevSnapIt->second.player->status;
				player.bomber.state = prevSnapIt->second.player->bomberData.state;
			}

			if (player.bomber.state & States::Bomber::Spawning && player.bomber.anim.spawnFlag) {
				player.bomber.anim.animateByType(Animations::Bomber::Spawn, player.bomber.anim.spawningDuration);
				player.bomber.anim.spawnFlag = false;
			}

			if (player.bomber.state & States::Bomber::Dying) {
				player.bomber.anim.animateByType(Animations::Bomber::Death, player.bomber.anim.dyingDuration);
				player.bomber.anim.spawnFlag = true;
			}
			else {
				if (cmd.movement & Command::Movement::Left) {
					player.moveLeft(delta);
					player.bomber.anim.animateByType(Animations::Bomber::Left, 12);
				}
				if (cmd.movement & Command::Movement::Right) {
					player.moveRight(delta);
					player.bomber.anim.animateByType(Animations::Bomber::Right, 12);
				}
				if (cmd.movement & Command::Movement::Down) {
					player.moveDown(delta);
					if (!(cmd.movement & Command::Movement::Right) && !(cmd.movement & Command::Movement::Left))
						player.bomber.anim.animateByType(Animations::Bomber::Down, 12);
				}
				if (cmd.movement & Command::Movement::Up) {
					player.moveUp(delta);
					if (!(cmd.movement & Command::Movement::Right) && !(cmd.movement & Command::Movement::Left))
						player.bomber.anim.animateByType(Animations::Bomber::Up, 12);
				}
			}

			player.bomber.anim.animate();
			player.bomber.sprite.setTextureRect(Math::getRectAtFrame(player.bomber.anim.selectedFrame, player.bomber.sprite.getTexture()->getSize().x, sf::Vector2<sf::Uint16>(20, 30)));
			posSnapshots.emplace_back(player.getBomber().getPosition());
		}

		void performInterpolation(sf::Uint64 tick)
		{
			if (worldSnapshots.size() > 1)
			{
				auto lowerBound = worldSnapshots.begin();
				auto upperBound = std::next(lowerBound);
				const auto upperEnd = worldSnapshots.end();
				const auto lowerEnd = std::prev(upperEnd);
				auto lastElemToBeDeleted = worldSnapshots.end();

				while (lowerBound != lowerEnd && upperBound != upperEnd) {
					if (lowerBound->first <= tick && tick < upperBound->first) {
						float lerpRatio = static_cast<float>(tick - lowerBound->first) / static_cast<float>(upperBound->first - lowerBound->first);

						updateEntities(lerpRatio, lowerBound->second.bombers, upperBound->second.bombers, bombers);
						updateEntities(lerpRatio, lowerBound->second.bombs, upperBound->second.bombs, bombs);
						updateEntities(lerpRatio, lowerBound->second.blocks, upperBound->second.blocks, blocks);

						break;
					}
					else {
						lastElemToBeDeleted = lowerBound;
					}

					++lowerBound;
					++upperBound;
				}

				if (lastElemToBeDeleted != worldSnapshots.end())
					worldSnapshots.erase(worldSnapshots.begin(), lastElemToBeDeleted + 1);
			}
		}

		void enqueueWorldSnapshot(const WorldSnapshot & worldSnapshot, sf::Uint64 tick)
		{
			worldSnapshots.emplace_back(tick, worldSnapshot);

			if (tick == 0) {
				auto & currentWorld = worldSnapshots.back().second;

				player = Player(*currentWorld.player);
				addAllEntities(*currentWorld.bombers, bombers);
				addAllEntities(*currentWorld.bombs, bombs);
				addAllEntities(*currentWorld.blocks, blocks);

				return;
			}

			if (worldSnapshots.size() > 1) {
				const auto nowSnapIt = std::prev(worldSnapshots.end());
				const auto prevSnapIt = std::prev(nowSnapIt);

				if (nowSnapIt->second.player == nullptr)
					nowSnapIt->second.player = std::make_shared<PlayerData>(*prevSnapIt->second.player);

				addPreviousEntities<UsersVector>(nowSnapIt->second.users, prevSnapIt->second.users);
				addPreviousEntities<BomberVector>(nowSnapIt->second.bombers, prevSnapIt->second.bombers, bombers);
				addPreviousEntities<BombVector>(nowSnapIt->second.bombs, prevSnapIt->second.bombs, bombs);
				addPreviousEntities<BlockVector>(nowSnapIt->second.blocks, prevSnapIt->second.blocks, blocks);

				clearAllDeadEntities(nowSnapIt->second.existingEntities);

				std::sort(worldSnapshots.begin(), worldSnapshots.end(),
						  [] (const std::pair<sf::Uint64, WorldSnapshot> & lhs, const std::pair<sf::Uint64, WorldSnapshot> & rhs) {
					return lhs.first < rhs.first;
				});
			}
		}

		UserData getPlayerUserData() const
		{
			UserData userData;

			userData.status = player.status;
			userData.tag = player.tag;

			return userData;
		}

		std::vector<UserData> getUsersData() const
		{
			std::vector<UserData> usersData;

			if (worldSnapshots.size() > 1) {
				const auto nowSnapIt = std::prev(worldSnapshots.end());
				const auto prevSnapIt = std::prev(nowSnapIt);

				for (const auto & user : *prevSnapIt->second.users) {
					usersData.emplace_back(user.second);
				}
			}

			return usersData;
		}

		const std::map<ID, Bomber> & getBombersData() const
		{
			return bombers;
		}

		const Player & getPlayer() const
		{
			return player;
		}

	private:
		typedef std::vector<std::pair<sf::Uint64, WorldSnapshot>> WorldSnps;
		WorldSnps worldSnapshots;

		Player player;
		std::map<ID, Bomber> bombers;
		std::map<ID, Bomb> bombs;
		std::map<ID, Block> blocks;

		void clearAllDeadEntities(const std::vector<ID> & existingEntities)
		{
			deleteDeadEntities(bombers, existingEntities);
			deleteDeadEntities(bombs, existingEntities);
			deleteDeadEntities(blocks, existingEntities);

			if (worldSnapshots.size() > 1) {
				const auto nowSnapIt = std::prev(worldSnapshots.end());
				const auto prevSnapIt = std::prev(nowSnapIt);

				deleteDeadEntities(*prevSnapIt->second.users, existingEntities);
				deleteDeadEntities(*nowSnapIt->second.users, existingEntities);
			}
		}

		template <typename T, typename M, typename E = M::mapped_type>
		void addAllEntities(const T & entityVector, M & entityContainer)
		{
			for (const auto & entity : entityVector)
				entityContainer.insert({entity.first, E(entity.second)});
		}

		template <typename T>
		void deleteDeadEntities(T & entityContainer, const std::vector<ID> & existingEntities)
		{
			auto iter = entityContainer.begin();
			auto endIter = entityContainer.end();

			for (; iter != endIter; ) {
				if (std::find(std::begin(existingEntities), std::end(existingEntities), iter->first) == existingEntities.end())
					entityContainer.erase(iter++);
				else
					++iter;
			}
		}

		template<typename C, typename T>
		void addPreviousEntities(T & entityPtr, T prevEntityPtr)
		{
			if (entityPtr == nullptr) {
				entityPtr = std::make_shared<C>(*prevEntityPtr);
			}
			else {
				const auto prevEntity = *prevEntityPtr;
				for (const auto & prevEntityData : prevEntity) {
					const auto it = std::find_if(entityPtr->begin(), entityPtr->end(), [&prevEntityData] (const decltype(prevEntityData) & other) {
						return prevEntityData.first == other.first;
					});

					if (it == entityPtr->end())
						entityPtr->emplace_back(prevEntityData);
				}
			}
		}

		template<typename C, typename T, typename D, typename M = D::mapped_type>
		void addPreviousEntities(T & entityPtr, T prevEntityPtr, D & entityContainer)
		{
			if (entityPtr == nullptr) {
				entityPtr = std::make_shared<C>(*prevEntityPtr);
			}
			else {
				const auto entity = *entityPtr;
				for (const auto & entityData : entity) {
					const auto it = std::find_if(prevEntityPtr->begin(), prevEntityPtr->end(), [&entityData] (const decltype(entityData) & other) {
						return entityData.first == other.first;
					});

					if (it == prevEntityPtr->end())
						entityContainer.insert({entityData.first, M(entityData.second)});
				}

				const auto prevEntity = *prevEntityPtr;
				for (const auto & prevEntityData : prevEntity) {
					const auto it = std::find_if(entityPtr->begin(), entityPtr->end(), [&prevEntityData] (const decltype(prevEntityData) & other) {
						return prevEntityData.first == other.first;
					});

					if (it == entityPtr->end())
						entityPtr->emplace_back(prevEntityData);
				}
			}
		}

		template <typename T, typename M>
		void updateEntities(float lerpRatio, T lowerSnap, T upperSnap, M & entityContainer)
		{
			const auto lowData = *lowerSnap;
			const auto upData = *upperSnap;

			for (auto & entity : entityContainer) {
				auto low = std::find_if(lowData.begin(), lowData.end(), [&entity] (const auto & lowEnt) { return lowEnt.first == entity.first; });
				auto up = std::find_if(upData.begin(), upData.end(), [&entity] (const auto & upEnt) { return upEnt.first == entity.first; });

				if (lowData.end() != low && upData.end() != up)
					entity.second.update(lerpRatio, low->second, up->second);
			}
		}

		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const
		{
			std::vector<std::pair<float, ID>> entitiesByPosition;

			std::for_each(bombers.begin(), bombers.end(), [&entitiesByPosition] (const auto & bomber)
			{ entitiesByPosition.emplace_back(bomber.second.sprite.getPosition().y, bomber.first); });
			std::for_each(bombs.begin(), bombs.end(), [&entitiesByPosition] (const auto & bomb)
			{ entitiesByPosition.emplace_back(bomb.second.sprite.getPosition().y, bomb.first); });
			std::for_each(blocks.begin(), blocks.end(), [&entitiesByPosition] (const auto & block)
			{ if (block.second.type != BlockType::Floor)
						  entitiesByPosition.emplace_back(block.second.sprite.getPosition().y, block.first);
			});
			entitiesByPosition.emplace_back(player.bomber.sprite.getPosition().y, player.tag.id);

			std::sort(entitiesByPosition.begin(), entitiesByPosition.end(), [] (const auto & lhs, const auto & rhs)
			{ return lhs.first < rhs.first; });

			for (const auto & block : blocks) {
				if (block.second.type == BlockType::Floor)
					target.draw(block.second);
			}
			for (const auto & entByPos : entitiesByPosition) {
				if (bombers.find(entByPos.second) != bombers.end())
					target.draw(bombers.at(entByPos.second));
				if (bombs.find(entByPos.second) != bombs.end())
					target.draw(bombs.at(entByPos.second));
				if (blocks.find(entByPos.second) != blocks.end())
					target.draw(blocks.at(entByPos.second));
				if (entByPos.second == player.tag.id)
					target.draw(player);
			}
		}
};