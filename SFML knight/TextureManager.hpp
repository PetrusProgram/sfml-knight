#pragma once
#include "ResourceManager.hpp"
#include "sfml\Graphics.hpp"


class TextureManager : public ResourceManager<TextureManager, sf::Texture>
{
public:
	TextureManager() : ResourceManager("/Resources/textures.cfg.txt") {}

	std::unique_ptr<sf::Texture> Load(const std::string& path) 
	{
		auto texture{ std::make_unique<sf::Texture>() };
		if (!texture->loadFromFile(Utils::GetWorkingDirectory() + path))
		{
			texture.reset();
			std::cerr << "Failed to load texture: " << path << std::endl;
		}
		return texture;
	}
};