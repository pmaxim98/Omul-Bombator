#pragma once

#include <SFML\Graphics.hpp>

#include "ResourceManager.h"
#include "Animations.h"

#include <BombData.h>
#include <MathUtils.h>
#include <States.h>

class Bomb : public sf::Drawable
{
	public:
		Bomb()
		{
			sprite.setTexture(ResourceManager::get().getTexture("Bombs"));
			sf::IntRect textureRect(sf::Vector2i(0, 0), sf::Vector2i(20, 20));
			sprite.setTextureRect(textureRect);
			sprite.setOrigin(textureRect.width / 2.0f, textureRect.height / 2.0f);
			sprite.setScale(2.0f, 2.0f);
		}

		Bomb(const BombData & bombData) : Bomb()
		{
			sprite.setPosition(bombData.position);
			sprite.setColor(bombData.color);
			owner = bombData.owner;
			ticksUntilExplosion = bombData.ticksUntilExplosion;
			sprite.setTextureRect(Math::getRectAtFrame(anim.selectedFrame, sprite.getTexture()->getSize().x, sf::Vector2<sf::Uint16>(20, 20)));
		}

		void update(float lerpRatio, const BombData & lowerSnap, const BombData & upperSnap)
		{
			sprite.setPosition(Math::lerp(lowerSnap.position, upperSnap.position, lerpRatio));
			sprite.setColor(lowerSnap.color);

			state = lowerSnap.state;
			if (state & States::Bomb::Ticking)
				anim.animateByType(Animations::Bomb::Tick, 15);

			anim.animate();

			sprite.setTextureRect(Math::getRectAtFrame(anim.selectedFrame, sprite.getTexture()->getSize().x, sf::Vector2<sf::Uint16>(20, 20)));
		}

		ID owner;
		sf::Sprite sprite;
		sf::Int16 damage;
		sf::Int16 range;
		sf::Int16 ticksUntilExplosion;
		sf::Uint32 state;
		Animations::BombAnim anim;

	private:
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const
		{
			target.draw(sprite, states);
		}
};
