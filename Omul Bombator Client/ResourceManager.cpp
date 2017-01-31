#include "ResourceManager.h"

#include "MathUtils.h"

#include <exception>

bool ResourceManager::Init()
{
	Math::initRandomGenerator();

	try {
		loadTexture("Bombers\\Bombers.png", "Bombers");
		loadTexture("Bombs\\Bombs.png", "Bombs");
		loadTexture("Blocks\\Explosions.png", "Explosions");
		loadTexture("Blocks\\Solids.png", "Solids");
		loadTexture("Blocks\\PowerUps4.png", "PowerUps");
		loadTexture("Hud\\Canvas2.png", "Canvas");
		loadFont("avqest.ttf", "Avqest");
		loadFont("segoeui.ttf", "Segoe");
	}
	catch (std::exception & e) {
		spdlog::get("console") -> error("Resource Manager Failure! {0}", e.what());
		return false;
	}

	animations.insert({Animations::Bomber, { 
			{Animations::Bomber::Down, {5, 4, 3}}, 
			{Animations::Bomber::Up, {6, 7, 8}},
			{Animations::Bomber::Left, {0, 1, 2}},
			{Animations::Bomber::Right, {9, 10, 11}},
			{Animations::Bomber::Spawn, {19, 20, 21}},
			{Animations::Bomber::Death, {12, 13, 14, 15, 16, 17, 18}}
		} 
	});

	animations.insert({Animations::Bomb, { 
			{Animations::Bomb::Tick, {0, 1, 2}}
		}
	});

	animations.insert({Animations::Block, { 
			{Animations::Block::CenterExplosion, {0, 1, 2, 3, 2, 1, 0}},
			{Animations::Block::LeftExplosion, {4, 5, 6, 7, 6, 5, 4}},
			{Animations::Block::RightExplosion, {16, 17, 18, 19, 18, 17, 16}},
			{Animations::Block::UpExplosion, {12, 13, 14, 15, 14, 13, 12}},
			{Animations::Block::DownExplosion, {24, 25, 26, 27, 26, 25, 24}},
			{Animations::Block::VerticalExplosion, {20, 21, 22, 23, 22, 21, 20}},
			{Animations::Block::HorizontalExplosion, {8, 9, 10, 11, 10, 9, 8}},
			{Animations::Block::Destruction, {2, 3, 4, 5}},
			{Animations::Block::FirePowerup, {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35}}, //python ftw
			{Animations::Block::MegaFirePowerup, {36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71}},
			{Animations::Block::BombPowerup, {72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107}},
			{Animations::Block::SpeedPowerup, {108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143}}
		}
	});
	
	return true;
} 

void ResourceManager::loadTexture(const std::string & filename, const std::string & id)
{
	sf::Texture tempTexture;

	if (!tempTexture.loadFromFile("..\\res\\Textures\\" + filename))
		throw std::runtime_error("Couldn't load the texture from '" + filename + "'.");
	
	tempTexture.setSmooth(false);
	textures.insert({id, tempTexture});
}

void ResourceManager::loadFont(const std::string & filename, const std::string & id)
{
	sf::Font tempFont;

	if (!tempFont.loadFromFile("..\\res\\Fonts\\" + filename))
		throw std::runtime_error("Couldn't load the font from '" + filename + "'.");

	fonts.insert({id, tempFont});
}

const sf::Texture & ResourceManager::getTexture(const std::string & id) const
{
	auto found = textures.find(id);

	if(found != textures.end())
		return found -> second;

	throw std::runtime_error("Texture with id " + id + " not found!");
}

const sf::Font & ResourceManager::getFont(const std::string & id) const
{
	auto found = fonts.find(id);

	if(found != fonts.end())
		return found -> second;

	throw std::runtime_error("Font with id " + id + " not found!");
}

const std::vector<sf::Uint16> & ResourceManager::getAnimation(Animations::Entities entityType, sf::Uint16 animationType)
{
	return animations[entityType][animationType];
}
