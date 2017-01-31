#pragma once

#include <SFML/Graphics.hpp>
#include <UserData.h>

#include "Bomber.h"
#include "Player.h"

class HUD : public sf::Drawable
{
	public:
		HUD(const sf::RenderWindow & window);

		sf::Text hp;
		sf::Text bombsCarry;
		sf::Text kills;
		sf::Text deaths;
		sf::Text speed;
		sf::Text damage;
		sf::Text range;
		sf::RectangleShape canvas;

		std::vector<sf::Text> nicknames;

		void update(const UserData & mainPlayerUserData, const std::vector<UserData> & otherUsersData, const std::map<ID, Bomber> & bombers, const Player & player);

	private:
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
};