#pragma once

#include <SFML\Graphics.hpp>

#include <PlayerData.h>
#include <PlayerTag.h>

#include "Animations.h"
#include "Bomber.h"

class Player : public sf::Drawable
{
	public:
		Player()
		{

		}

		Player(const PlayerData & data)
		{
			status = data.status;
			tag = data.tag;
			bomber = data.bomberData;
			bomber.anim.animateByType(Animations::Bomber::Spawn, 27);
		}

		void moveDown(float dt)
		{
			bomber.moveDown(dt, status.speed);
		}

		void moveUp(float dt)
		{
			bomber.moveUp(dt, status.speed);
		}

		void moveLeft(float dt)
		{
			bomber.moveLeft(dt, status.speed);
		}

		void moveRight(float dt)
		{
			bomber.moveRight(dt, status.speed);
		}

		void setPosition(sf::Vector2f pos)
		{
			bomber.setPosition(pos);
		}

		const PlayerStatus & getPlayerStatus() const
		{
			return status;
		}

		const PlayerTag & getPlayerTag() const
		{
			return tag;
		}

		const Bomber & getBomber() const
		{
			return bomber;
		}

		PlayerStatus status;
		PlayerTag tag;
		Bomber bomber;

	private:
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const
		{
			target.draw(bomber, states);
		}
};
