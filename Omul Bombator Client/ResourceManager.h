#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Font.hpp>

#include <spdlog\spdlog.h>

#include <map>
#include <string>

#include "Animations.h"

class ResourceManager
{
	public:
		ResourceManager(const ResourceManager &) = delete;
		void operator=(const ResourceManager &) = delete;	

		static ResourceManager & get()
        {
            static ResourceManager instance;
            return instance;
        }

		const sf::Texture & getTexture(const std::string & id) const;
		const sf::Font & getFont(const std::string & id) const;
		const std::vector<sf::Uint16> & getAnimation(Animations::Entities entityType, sf::Uint16 animationType);

		bool Init();

	private:
		ResourceManager() = default;

		void loadTexture(const std::string & filename, const std::string & id);
		void loadFont(const std::string & filename, const std::string & id);

		std::map<std::string, sf::Texture> textures;
		std::map<std::string, sf::Font> fonts;
		std::map<Animations::Entities, std::map<sf::Uint16, std::vector<sf::Uint16>>> animations;
};

#endif