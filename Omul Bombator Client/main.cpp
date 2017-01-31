#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>
#include <iostream>
#include <TGUI\TGUI.hpp>

#include "Client.h"
#include "World.h"
#include "HUD.h"
#include "ResourceManager.h"

struct ConnectingData
{
	sf::IpAddress serverIP;
	std::string nickname;
	sf::Color bomberColor;
	bool connected;
} connectingData;

void connect(tgui::EditBox::Ptr ip, tgui::EditBox::Ptr nickname, tgui::ComboBox::Ptr color)
{
	connectingData.serverIP = sf::IpAddress(ip->getText());
	connectingData.nickname = nickname->getText();
	connectingData.bomberColor = tgui::Color(color->getSelectedItem());
	connectingData.connected = true;
}

void loadWidgets(tgui::Gui& gui)
{
	auto theme = std::make_shared<tgui::Theme>();

	auto windowWidth = tgui::bindWidth(gui);
	auto windowHeight = tgui::bindHeight(gui);

	tgui::EditBox::Ptr editBoxNickname = theme->load("EditBox");
	editBoxNickname->setSize(windowWidth / 3, windowHeight / 20);
	editBoxNickname->setPosition(windowWidth / 2 - editBoxNickname->getSize().x / 2, windowHeight / 4 + 50);
	editBoxNickname->setDefaultText("Nickname");
	editBoxNickname->setTextSize(12);
	gui.add(editBoxNickname, "Nickname");

	tgui::EditBox::Ptr editBoxIP = theme->load("EditBox");
	editBoxIP->setSize(windowWidth / 3, windowHeight / 20);
	editBoxIP->setPosition(windowWidth / 2 - editBoxIP->getSize().x / 2, windowHeight / 4 + 100);
	editBoxIP->setDefaultText("IP");
	editBoxIP->setTextSize(12);
	gui.add(editBoxIP, "ServerIP");

	tgui::ComboBox::Ptr comboBox = theme->load("ComboBox");
	comboBox->addItem("Black");
	comboBox->addItem("White");
	comboBox->addItem("Red");
	comboBox->addItem("Yellow");
	comboBox->addItem("Blue");
	comboBox->addItem("Green");
	comboBox->addItem("Cyan");
	comboBox->addItem("Magenta");
	
	comboBox->setFont(tgui::Font(ResourceManager::get().getFont("Avqest")));
	comboBox->setSelectedItemByIndex(0);
	comboBox->setTextSize(14);
	comboBox->setPosition(windowWidth / 2 - comboBox->getSize().x / 2, windowHeight / 4 + 150);
	gui.add(comboBox);
	
	tgui::Button::Ptr buttonJoin = theme->load("Button");
	buttonJoin->setSize(windowWidth / 6, windowHeight / 18);
	buttonJoin->setPosition(windowWidth / 2 - buttonJoin->getSize().x / 2, windowHeight / 4 + 200);
	buttonJoin->setFont(tgui::Font(ResourceManager::get().getFont("Avqest")));
	buttonJoin->setText("Join");
	buttonJoin->setTextSize(16);
	gui.add(buttonJoin);

	buttonJoin->connect("pressed", connect, editBoxIP, editBoxNickname, comboBox);
}

int main()
{
	auto console = spdlog::stdout_color_mt("console");

	if (!ResourceManager::get().Init()) {
		console->error("Failed initializing the resource manager!");
		return 0;
	}

	sf::RenderWindow window(sf::VideoMode(600, 600), "Client", sf::Style::Close);
	window.setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width - window.getSize().x) / 2, (sf::VideoMode::getDesktopMode().height - window.getSize().y) / 2 - 40));
	tgui::Gui gui(window);

	try
	{
		loadWidgets(gui);
	}
	catch (const tgui::Exception& e)
	{
		std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
		std::getchar();
		//return 1;
	}

	while (window.isOpen())
	{
		if (connectingData.connected)
			break;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			gui.handleEvent(event);
		}

		window.clear();
		gui.draw();
		window.display();
	}


	WorldSnapshot firstWorldSnapshot;

	Client client(connectingData.serverIP, connectingData.nickname, connectingData.bomberColor, firstWorldSnapshot);

	World world;
	HUD hud(window);

	sf::Clock clientClock;

	world.enqueueWorldSnapshot(firstWorldSnapshot, 0);

	sf::Time timeSinceLastInputUpdate = sf::Time::Zero;
	sf::Time timeSinceLastWorldUpdate = sf::Time::Zero;
	sf::Time accSinceLastInputUpdate = sf::Time::Zero;

	sf::Uint64 currentTick = 0;
	const float tickRate = 60.0f;
	const float serverTickRate = 30.0f;
	const float inputRate = 30.0f;
	const sf::Time timeStep = sf::seconds(1.0f / tickRate);
	const sf::Time inputStep = sf::seconds(1.0f / inputRate);
	const sf::Time serverTimeStep = sf::seconds(1.0f / serverTickRate);
	const sf::Time interp = sf::seconds(1.0f / 10.0f);
	const sf::Uint64 interpTicks = interp.asMilliseconds() / timeStep.asMilliseconds();

	sf::Clock worldTimer;
	sf::Clock inputTimer;

	while (window.isOpen())
	{
		//spdlog::get("console") -> error("Tick: {0}.", currentTick);

		timeSinceLastInputUpdate += accSinceLastInputUpdate;

		Command::CMD cmd;

		while (timeSinceLastInputUpdate > inputStep)
		{
			timeSinceLastInputUpdate -= inputStep;

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					cmd.network |= Command::Network::Leave;
					client.sendCommands(cmd);
					window.close();
					return 0;
				}

				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Num1) {
						cmd.appearance |= Command::Appearance::Color;
						cmd.additionalData.color = sf::Color::Red;
					}

					if (event.key.code == sf::Keyboard::Num2) {
						cmd.appearance |= Command::Appearance::Color;
						cmd.additionalData.color = sf::Color::Green;
					}
				}
			}

			if (window.hasFocus()) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) cmd.movement |= Command::Movement::Up;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) cmd.movement |= Command::Movement::Down;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) cmd.movement |= Command::Movement::Right;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) cmd.movement |= Command::Movement::Left;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) cmd.action |= Command::Action::Plant;
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) cmd.action |= Command::Action::Detonate;
			}

			client.sendCommands(cmd);
		}

		accSinceLastInputUpdate = inputTimer.restart();

		timeSinceLastWorldUpdate += worldTimer.restart();

		while (timeSinceLastWorldUpdate > timeStep)
		{
			timeSinceLastWorldUpdate -= timeStep;

			WorldSnapshot worldSnapshot;
			if (client.receiveWorldSnapshot(worldSnapshot))
				world.enqueueWorldSnapshot(worldSnapshot, currentTick);

			if (currentTick > interpTicks) {
				world.update(serverTimeStep.asSeconds(), currentTick - interpTicks, cmd);
				hud.update(world.getPlayerUserData(), world.getUsersData(), world.getBombersData(), world.getPlayer());

				window.clear(sf::Color::White);
				window.draw(world);
				window.draw(hud);
				window.display();
			}
		}

		if (timeStep >= timeSinceLastWorldUpdate) {
			std::chrono::milliseconds timeUntilNextFrame(sf::Time(timeStep - timeSinceLastWorldUpdate).asMilliseconds());
			std::this_thread::sleep_for(timeUntilNextFrame);
		}

		currentTick++;
	}

	return 0;
}