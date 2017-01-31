#pragma once

#include <SFML\Graphics.hpp>

#include "ResourceManager.h"
#include "Animations.h"

#include <BlockData.h>
#include <BlockType.h>
#include <MathUtils.h>
#include <States.h>

class Block : public sf::Drawable
{
	public:
		Block()
		{

		}

		Block(const BlockData & blockData)
			:
			anim(28)
		{
			type = blockData.type;
			state = blockData.state;

			if (type == BlockType::Wall) {
				sprite.setTexture(ResourceManager::get().getTexture("Solids"));
				sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
				sprite.setOrigin(32, 32);
				sprite.setScale(50.0f / 64.0f, 50.0f / 64.0f);
				sprite.setPosition((blockData.position.y + 1) * 50 - 25, (blockData.position.x + 1) * 50 - 25);
			}
			else if (type == BlockType::Obstacle) {
				sprite.setTexture(ResourceManager::get().getTexture("Solids"));
				sprite.setTextureRect(sf::IntRect(64, 0, 64, 64));
				sprite.setOrigin(32, 32);
				sprite.setScale(50.0f / 64.0f, 50.0f / 64.0f);
				sprite.setPosition((blockData.position.y + 1) * 50 - 25, (blockData.position.x + 1) * 50 - 25);
			}
			else if (type == BlockType::Floor) {
				sprite.setTexture(ResourceManager::get().getTexture("Solids"));
				sprite.setTextureRect(sf::IntRect(0, 64, 32, 32));
				sprite.setOrigin(16, 0);
				sprite.setScale(50.0f / 32.0f, 50.0f / 32.0f);
				sprite.setPosition((blockData.position.y + 1) * 50 - 25, (blockData.position.x + 1) * 50 - 25 - 25);
			}
			else if (type == BlockType::Explosion) {
				sprite.setTexture(ResourceManager::get().getTexture("Explosions"));
				sprite.setTextureRect(sf::IntRect(80, 40, 20, 20));
				sprite.setOrigin(10, 10);
				sprite.setScale(50.0f / 20.0f, 50.0f / 20.0f);
				sprite.setPosition((blockData.position.y + 1) * 50 - 25, (blockData.position.x + 1) * 50 - 25);
			}
			else if (type == BlockType::PowerUp) {
				sprite.setTexture(ResourceManager::get().getTexture("PowerUps"));

				if (state == States::Block::FirePowerupGlowing)
					sprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
				if (state == States::Block::MegaFirePowerupGlowing)
					sprite.setTextureRect(sf::IntRect(0, 50, 50, 50));
				if (state == States::Block::BombPowerupGlowing)
					sprite.setTextureRect(sf::IntRect(0, 100, 50, 50));
				if (state == States::Block::SpeedPowerupGlowing)
					sprite.setTextureRect(sf::IntRect(0, 150, 50, 50));

				sprite.setOrigin(25, 25);
				sprite.setPosition((blockData.position.y + 1) * 50 - 25, (blockData.position.x + 1) * 50 - 25);
			}
		}

		void update(float lerpRatio, const BlockData & lowerSnap, const BlockData & upperSnap)
		{
			if (lowerSnap.type == BlockType::Explosion) {
				state = lowerSnap.state;

				sf::Vector2<sf::Uint16> tileSize = {20, 20};

				if (state == States::Block::CenterExploding)
					anim.animateByType(Animations::Block::CenterExplosion, anim.explosionDuration);
				else if (state == States::Block::VerticalExploding)
					anim.animateByType(Animations::Block::VerticalExplosion, anim.explosionDuration);
				else if (state == States::Block::HorizontalExploding)
					anim.animateByType(Animations::Block::HorizontalExplosion, anim.explosionDuration);
				else if (state == States::Block::UpExploding)
					anim.animateByType(Animations::Block::UpExplosion, anim.explosionDuration);
				else if (state == States::Block::LeftExploding)
					anim.animateByType(Animations::Block::LeftExplosion, anim.explosionDuration);
				else if (state == States::Block::DownExploding)
					anim.animateByType(Animations::Block::DownExplosion, anim.explosionDuration);
				else if (state == States::Block::RightExploding)
					anim.animateByType(Animations::Block::RightExplosion, anim.explosionDuration);

				anim.animate();

				sprite.setTextureRect(Math::getRectAtFrame(anim.selectedFrame, sprite.getTexture()->getSize().x, tileSize));
				sprite.setOrigin(static_cast<float>(sprite.getTextureRect().width / 2), static_cast<float>(sprite.getTextureRect().height / 2));
			}
			else if (lowerSnap.type == BlockType::Obstacle) {
				state = lowerSnap.state;

				sf::Vector2<sf::Uint16> tileSize = {64, 64};

				if (state == States::Block::Destructing) {
					anim.animateByType(Animations::Block::Destruction, anim.destructionDuration);

					anim.animate();

					sprite.setTextureRect(Math::getRectAtFrame(anim.selectedFrame, sprite.getTexture()->getSize().x, tileSize));
					sprite.setOrigin(static_cast<float>(sprite.getTextureRect().width / 2), static_cast<float>(sprite.getTextureRect().height / 2));
				}
			}
			else if (lowerSnap.type == BlockType::PowerUp) {
				state = lowerSnap.state;

				sf::Vector2<sf::Uint16> tileSize = {50, 50};

				if (state == States::Block::FirePowerupGlowing)
					anim.animateByType(Animations::Block::FirePowerup, anim.powerupDuration);
				else if (state == States::Block::MegaFirePowerupGlowing)
					anim.animateByType(Animations::Block::MegaFirePowerup, anim.powerupDuration);
				else if (state == States::Block::BombPowerupGlowing)
					anim.animateByType(Animations::Block::BombPowerup, anim.powerupDuration);
				else if (state == States::Block::SpeedPowerupGlowing)
					anim.animateByType(Animations::Block::SpeedPowerup, anim.powerupDuration);

				anim.animate();

				sprite.setTextureRect(Math::getRectAtFrame(anim.selectedFrame, sprite.getTexture()->getSize().x, tileSize));
			}
		}

		sf::Uint8 type;
		sf::Uint32 state;
		sf::Sprite sprite;
		Animations::BlockAnim anim;

	private:
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const
		{
			target.draw(sprite, states);
		}
};