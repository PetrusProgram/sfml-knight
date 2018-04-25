#pragma once
#include "sfml\Graphics.hpp"
#include "Utilities.hpp"
#include "BaseState.hpp"
#include "Tiles.hpp"
#include <unordered_map>
#include <map>
#include <array>
#include <fstream>
#include <sstream>

using TileID = unsigned int;
using TileMap = std::unordered_map<TileID, std::unique_ptr<Tile>>;
using TileSet = std::unordered_map<TileID, std::unique_ptr<TileInfo>>;

class Map {
public:
	Map(SharedContext* context, BaseState* currentState);
	~Map();

	Tile* GetTile(unsigned int x, unsigned int y);
	TileInfo* GetDefaultTile() { return &m_defaultTile; }

	float GetGravity()const { return m_mapGravity; }
	unsigned int GetTileSize()const { return Sheet::Tile_Size; }
	const sf::Vector2u& GetMapSize()const { return m_maxMapSize; }
	const sf::Vector2f& GetPlayerStart()const { return m_playerStart; }

	void LoadMap(const std::string& path);
	void LoadNext() { m_loadNextMap = true; }

	void Update(float dT);
	void Draw();
private:
	unsigned int ConvertCoords(unsigned int x, unsigned int y) { return (x * m_maxMapSize.x) + y;  }
	void LoadTiles(const std::string& path);
	void PurgeMap();
	void PurgeTileSet();

	TileSet m_tileSet;
	TileMap m_tileMap;
	sf::Sprite m_background;
	TileInfo m_defaultTile;
	sf::Vector2u m_maxMapSize{ 32u, 32u };
	sf::Vector2f m_playerStart;
	unsigned int m_tileCount{ 0u };
	unsigned int m_tileSetCount{ 0u };
	float m_mapGravity{ 512.f };
	std::string m_nextMap;
	bool m_loadNextMap{ false };
	std::string m_backgroundTexture;
	BaseState* m_currentState{ nullptr };
	SharedContext* m_context{ nullptr };
};