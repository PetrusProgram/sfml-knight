#pragma once
#include "SharedContext.hpp"

enum Sheet
{
	Tile_Size = 32,
	Sheet_Width = 256,
	Sheet_Height = 256
};

struct TileInfo {
	TileInfo(SharedContext* context, const std::string& texture = "", unsigned int id = 0);

	~TileInfo();

	sf::Sprite m_sprite;

	unsigned int m_id{ 0u };
	std::string m_name;
	sf::Vector2f m_friction;
	bool m_deadly{ false };

	SharedContext* m_context{ nullptr };
	std::string m_texture;
};

struct Tile {
	TileInfo* m_properties{ nullptr };
	bool m_warp{ false };
};