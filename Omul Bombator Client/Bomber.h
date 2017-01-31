#pragma once

#include <SFML\Graphics.hpp>

#include "ResourceManager.h"
#include "Animations.h"

#include <BomberData.h>
#include <MathUtils.h>
#include <States.h>

class Bomber : public sf::Drawable
{
	public:
		Bomber()
		{
			sprite.setTexture(ResourceManager::get().getTexture("Bombers"));
			sprite.setScale(2.0f, 2.0f);
		}

		Bomber(const BomberData & bomberData) : Bomber()
		{
			sprite.setColor(bomberData.color);
			state = bomberData.state;
			sprite.setTextureRect(Math::getRectAtFrame(anim.selectedFrame, sprite.getTexture()->getSize().x, sf::Vector2<sf::Uint16>(20, 30)));
			sprite.setOrigin(10.0f, 15.0f);
			sprite.setPosition(bomberData.position);
		}

		void update(float lerpRatio, const BomberData & lowerSnap, const BomberData & upperSnap)
		{
			auto newPos = Math::lerp(lowerSnap.position, upperSnap.position, lerpRatio);
			sprite.setPosition(std::floor(newPos.x), std::floor(newPos.y));
			sprite.setColor(lowerSnap.color);

			state = lowerSnap.state;

			if (state & States::Bomber::Spawning && anim.spawnFlag) {
				anim.animateByType(Animations::Bomber::Spawn, anim.spawningDuration);
				anim.spawnFlag = false;
			}

			if (lowerSnap.position != upperSnap.position) {
				if (state & States::Bomber::Left)
					anim.animateByType(Animations::Bomber::Left, anim.movementDuration);
				else if (state & States::Bomber::Right)
					anim.animateByType(Animations::Bomber::Right, anim.movementDuration);
				else if ((state & States::Bomber::Up) || (state & (States::Bomber::Up | States::Bomber::Right)) || (state & (States::Bomber::Up | States::Bomber::Left)))
					anim.animateByType(Animations::Bomber::Up, anim.movementDuration);
				else if ((state & States::Bomber::Down) || (state & (States::Bomber::Down | States::Bomber::Right)) || (state & (States::Bomber::Down | States::Bomber::Left)))
					anim.animateByType(Animations::Bomber::Down, anim.movementDuration);
			}

			if (state & States::Bomber::Dying) {
				anim.animateByType(Animations::Bomber::Death, anim.dyingDuration);
				anim.spawnFlag = true;
			}

			anim.animate();

			sprite.setTextureRect(Math::getRectAtFrame(anim.selectedFrame, sprite.getTexture()->getSize().x, sf::Vector2<sf::Uint16>(20, 30)));
			sprite.setOrigin(static_cast<float>(sprite.getTextureRect().width / 2), static_cast<float>(sprite.getTextureRect().height / 2));
		}

		void moveDown(float dt, float velocity)
		{
			sprite.move(sf::Vector2f(0.0f, dt * velocity));
		}

		void moveUp(float dt, float velocity)
		{
			sprite.move(sf::Vector2f(0.0f, -dt * velocity));
		}

		void moveLeft(float dt, float velocity)
		{
			sprite.move(sf::Vector2f(-dt * velocity, 0.0f));
		}

		void moveRight(float dt, float velocity)
		{
			sprite.move(sf::Vector2f(dt * velocity, 0.0f));
		}

		sf::Vector2f getPosition() const
		{
			return sprite.getPosition();
		}

		void setPosition(sf::Vector2f pos)
		{
			sprite.setPosition(pos);
		}

		sf::Sprite sprite;
		sf::Uint32 state;
		Animations::BomberAnim anim;

	private:
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const
		{
			target.draw(sprite, states);
		}
};