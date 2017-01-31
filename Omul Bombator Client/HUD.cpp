#include "HUD.h"

#include "ResourceManager.h"

HUD::HUD(const sf::RenderWindow & window)
{
	const auto & defaultFont = ResourceManager::get().getFont("Avqest");

	canvas.setSize({600.0f, 100.0f});
	canvas.setPosition({0.0f, 500.0f});
	canvas.setOutlineThickness(1.0f);
	canvas.setOutlineColor(sf::Color::Black);
	canvas.setTexture(&ResourceManager::get().getTexture("Canvas"));

	hp.setFont(defaultFont);
	hp.setCharacterSize(22);
	hp.setString("0");
	hp.setOrigin(0, hp.getGlobalBounds().height);
	hp.setPosition(sf::Vector2f(20.0f, window.getSize().y - 35.0f));
	hp.setFillColor(sf::Color::Cyan);
	hp.setOutlineColor(sf::Color::Black);
	hp.setOutlineThickness(1.0f);

	bombsCarry.setFont(defaultFont);
	bombsCarry.setCharacterSize(22);
	bombsCarry.setString("0");
	bombsCarry.setOrigin(0, bombsCarry.getGlobalBounds().height);
	bombsCarry.setPosition(sf::Vector2f(135.0f, window.getSize().y - 35.0f));
	bombsCarry.setFillColor(sf::Color::Cyan);
	bombsCarry.setOutlineColor(sf::Color::Black);
	bombsCarry.setOutlineThickness(1.0f);

	speed.setFont(defaultFont);
	speed.setCharacterSize(22);
	speed.setString("0");
	speed.setOrigin(0, speed.getGlobalBounds().height);
	speed.setPosition(sf::Vector2f(250.0f, window.getSize().y - 35.0f));
	speed.setFillColor(sf::Color::Cyan);
	speed.setOutlineColor(sf::Color::Black);
	speed.setOutlineThickness(1.0f);

	deaths.setFont(defaultFont);
	deaths.setCharacterSize(22);
	deaths.setString("0");
	deaths.setOrigin(deaths.getGlobalBounds().width, deaths.getGlobalBounds().height);
	deaths.setPosition(sf::Vector2f(window.getSize().x - 40.0f, window.getSize().y - 35.0f));
	deaths.setFillColor(sf::Color::Red);
	deaths.setOutlineColor(sf::Color::Black);
	deaths.setOutlineThickness(1.0f);

	kills.setFont(defaultFont);
	kills.setCharacterSize(21);
	kills.setString("0");
	kills.setOrigin(kills.getGlobalBounds().width, kills.getGlobalBounds().height);
	kills.setPosition(sf::Vector2f(window.getSize().x - 80.0f, window.getSize().y - 35.0f));
	kills.setFillColor(sf::Color::Green);
	kills.setOutlineColor(sf::Color::Black);
	kills.setOutlineThickness(1.0f);

	damage.setFont(defaultFont);
	damage.setCharacterSize(21);
	damage.setString("0");
	damage.setOrigin(damage.getGlobalBounds().width, range.getGlobalBounds().height);
	damage.setPosition(sf::Vector2f(window.getSize().x - 80.0f, window.getSize().y - 35.0f));
	damage.setFillColor(sf::Color::Cyan);
	damage.setOutlineColor(sf::Color::Black);
	damage.setOutlineThickness(1.0f);

	range.setFont(defaultFont);
	range.setCharacterSize(21);
	range.setString("0");
	range.setOrigin(range.getGlobalBounds().width, range.getGlobalBounds().height);
	range.setPosition(sf::Vector2f(window.getSize().x - 80.0f, window.getSize().y - 35.0f));
	range.setFillColor(sf::Color::Cyan);
	range.setOutlineColor(sf::Color::Black);
	range.setOutlineThickness(1.0f);
}

void HUD::update(const UserData & mainPlayerUserData, const std::vector<UserData> & otherUsersData, const std::map<ID, Bomber> & bombers, const Player & player)
{
	auto center = [] (sf::Text & text, const std::string & string, const sf::Vector2f & pos) {
		text.setString(string);
		text.setOrigin(std::floor(text.getGlobalBounds().width / 2), std::floor(text.getGlobalBounds().height / 2));
		text.setPosition(pos);
	};

	center(hp, std::to_string(mainPlayerUserData.status.hp), sf::Vector2f(41.0f, hp.getPosition().y));

	if (mainPlayerUserData.status.hp < 25)
		hp.setFillColor(sf::Color::Red);
	else if (mainPlayerUserData.status.hp < 50)
		hp.setFillColor(sf::Color::Yellow);
	else if (mainPlayerUserData.status.hp < 75)
		hp.setFillColor(sf::Color::Green);
	else if (mainPlayerUserData.status.hp <= 100)
		hp.setFillColor(sf::Color::Cyan);

	center(kills, std::to_string(mainPlayerUserData.status.kills), sf::Vector2f(455.0f, kills.getPosition().y));
	center(deaths, std::to_string(mainPlayerUserData.status.deaths), sf::Vector2f(535.0f, deaths.getPosition().y));
	center(speed, std::to_string(mainPlayerUserData.status.speed), sf::Vector2f(195.0f, speed.getPosition().y));
	center(bombsCarry, std::to_string(mainPlayerUserData.status.bombsCarryLimit), sf::Vector2f(109.0f, bombsCarry.getPosition().y));
	center(range, std::to_string(mainPlayerUserData.status.bombsRange), sf::Vector2f(380.0f, speed.getPosition().y));
	center(damage, std::to_string(mainPlayerUserData.status.bombsDamage), sf::Vector2f(288.0f, bombsCarry.getPosition().y));

	nicknames.clear();

	for (const auto & otherPlayer : otherUsersData) {
		sf::Text nick;

		sf::Vector2f bomberPos = bombers.at(otherPlayer.tag.id).sprite.getPosition();
		sf::Color bomberColor = bombers.at(otherPlayer.tag.id).sprite.getColor();

		nick.setFont(ResourceManager::get().getFont("Avqest"));
		nick.setCharacterSize(20);
		nick.setString(otherPlayer.tag.nickname);
		nick.setOrigin(nick.getGlobalBounds().width / 2, nick.getGlobalBounds().height / 2);
		nick.setPosition(sf::Vector2f(std::floor(bomberPos.x), std::floor(bomberPos.y - 45.0f)));
		nick.setFillColor(bomberColor);
		nick.setOutlineColor(sf::Color::Black);
		nick.setOutlineThickness(1.0f);

		nicknames.emplace_back(nick);
	}

	sf::Text nick;

	sf::Vector2f bomberPos = player.bomber.sprite.getPosition();
	sf::Color bomberColor = player.bomber.sprite.getColor();

	nick.setFont(ResourceManager::get().getFont("Avqest"));
	nick.setCharacterSize(22);
	nick.setString(mainPlayerUserData.tag.nickname);
	nick.setOrigin(nick.getLocalBounds().width / 2, nick.getLocalBounds().height / 2);
	nick.setPosition(sf::Vector2f(std::floor(bomberPos.x), std::floor(bomberPos.y - 45.0f)));
	nick.setFillColor(bomberColor);
	nick.setOutlineColor(sf::Color::Black);
	nick.setOutlineThickness(1.0f);

	nicknames.emplace_back(nick);
}

void HUD::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(canvas, states);
	target.draw(hp, states);
	target.draw(bombsCarry, states);
	target.draw(range, states);
	target.draw(damage, states);
	target.draw(speed, states);
	target.draw(kills, states);
	target.draw(deaths, states);

	for (const auto & nickname : nicknames)
		target.draw(nickname, states);
}
