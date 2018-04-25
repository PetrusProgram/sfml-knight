#include "Map.hpp"
#include "StateManager.hpp"

Map::Map(SharedContext* context, BaseState* currentState) :m_context(context), m_defaultTile(context), m_currentState(currentState)
{
	m_tileMap.reserve(292);
	m_context->m_gameMap = this;
	LoadTiles("tiles.cfg.txt");
}

Map::~Map() 
{
	PurgeMap();
	PurgeTileSet();
	m_context->m_gameMap = nullptr;
}

Tile* Map::GetTile(unsigned int x, unsigned int y) 
{

	auto itr{ m_tileMap.find(ConvertCoords(x, y)) };
	return(itr != m_tileMap.end() ? itr->second.get() : nullptr);
}

void Map::LoadMap(const std::string& path) 
{
	std::ifstream mapFile(Utils::GetWorkingDirectory() + path);
	if (!mapFile)
	{ 
		std::cout << "Failed loading map file: " << path << std::endl;
		return; 
	}
	auto entityMgr{ m_context->m_entityManager };
	std::string line;
	std::cout << "Loading a map: " << path << std::endl;

	int playerId{ -1 };
	while (std::getline(mapFile, line))
	{
		if (line[0] == '|') 
		 continue; 

		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "TILE")
		{
			int tileId{ 0 };
			keystream >> tileId;
			if (tileId < 0) 
			{ 
				std::cout << "! Bad tile id: " << tileId << std::endl; 
				continue; 
			}
			auto itr{ m_tileSet.find(tileId) };
			if (itr == m_tileSet.end()) 
			{ 
				std::cout << "Tile id(" << tileId << ") was not found in tileset." << std::endl;
				continue; 
			}
			sf::Vector2i tileCoords;
			keystream >> tileCoords.x >> tileCoords.y;
			if (tileCoords.x > m_maxMapSize.x || tileCoords.y > m_maxMapSize.y) 
			{
				std::cout << "Tile is out of range: " << tileCoords.x << " " << tileCoords.y << std::endl;
				continue;
			}
			auto tileMove{ std::make_unique<Tile>() };
			auto tile{ tileMove.get() };

			tile->m_properties = itr->second.get();
			if (!m_tileMap.emplace(ConvertCoords(tileCoords.x, tileCoords.y), std::move(tileMove)).second)
			{

				std::cout << "! Duplicate tile! : " << tileCoords.x
					<< "" << tileCoords.y << std::endl;
				tile = nullptr;
				continue;
			}
			std::string warp;
			keystream >> warp;
			if (warp == "WARP")  
				tile->m_warp = true; 
		}
		else if (type == "BACKGROUND") 
		{
			if (m_backgroundTexture != "") 
				continue; 

			keystream >> m_backgroundTexture;
			if (!m_context->m_textureManager->RequireResource(m_backgroundTexture))
			{
				m_backgroundTexture = "";
				continue;
			}
			auto texture{ m_context->m_textureManager->GetResource(m_backgroundTexture) };
			m_background.setTexture(*texture);
			auto viewSize{ m_currentState->GetView().getSize() };
			auto textureSize{ texture->getSize() };
			sf::Vector2f scaleFactors;
			scaleFactors.x = viewSize.x / textureSize.x;
			scaleFactors.y = viewSize.y / textureSize.y;
			m_background.setScale(scaleFactors);
		}
		else if (type == "SIZE") 
			keystream >> m_maxMapSize.x >> m_maxMapSize.y;
		
		else if (type == "GRAVITY") 
			keystream >> m_mapGravity;
		
		else if (type == "DEFAULT_FRICTION") 
			keystream >> m_defaultTile.m_friction.x >> m_defaultTile.m_friction.y;

		else if (type == "NEXTMAP") 
			keystream >> m_nextMap;

		else if (type == "PLAYER") 
		{
			if (playerId != -1)
				continue;

			playerId = entityMgr->Add(EntityType::Player);
			if (playerId < 0)
				continue;

			float playerX{ 0.f };
			float playerY{ 0.f };
			keystream >> playerX >> playerY;
			entityMgr->Find(playerId)->SetPosition(playerX, playerY);
			m_playerStart = sf::Vector2f(playerX, playerY);
		}
		else if (type == "ENEMY") 
		{
			std::string enemyName;
			keystream >> enemyName;
			int enemyId = entityMgr->Add(EntityType::Enemy, enemyName);
			if (enemyId < 0)
				continue;

			float enemyX{ 0.f };
			float enemyY{ 0.f };
			keystream >> enemyX >> enemyY;
			entityMgr->Find(enemyId)->SetPosition(enemyX, enemyY);
		}
		else
			std::cout << "Unknown type \"" << type << "\"." << std::endl;
	}
	std::cout << "Map Loaded!" << std::endl;
}


void Map::LoadTiles(const std::string& path)
{
	std::ifstream file(Utils::GetWorkingDirectory() + std::string("/Resources/") + path);
	if (!file)
	{
		std::cout << "Failed loading tile set file: " << path << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|')  
			continue; 

		std::stringstream keystream(line);
		int tileId;
		keystream >> tileId;
		if (tileId < 0)  
			continue; 

		auto tile{ std::make_unique<TileInfo>(m_context, "TileSheet", tileId) };
		keystream >> tile->m_name >> tile->m_friction.x >> tile->m_friction.y >> tile->m_deadly;
		if (!m_tileSet.emplace(tileId, std::move(tile)).second) 
			std::cout << "Duplicate tile type: " << tile->m_name << std::endl;
	}
}

void Map::Update(float dT) 
{
	if (m_loadNextMap)
	{
		PurgeMap();
		m_loadNextMap = false;
		if (m_nextMap != "") 
			LoadMap("/Resources/media/maps/" + m_nextMap);
	
		else 
			m_currentState->GetStateManager()->SwitchTo(StateType::GameOver);

		m_nextMap = "";
	}
	auto viewSpace{ m_context->m_wind->GetViewSpace() };
	m_background.setPosition(viewSpace.left, viewSpace.top);
}

void Map::Draw() 
{
	auto wind{ m_context->m_wind->GetRenderWindow() };
	wind->draw(m_background);
	auto viewSpace{ m_context->m_wind->GetViewSpace() };

	sf::Vector2i tileBegin(floor(viewSpace.left / Sheet::Tile_Size), floor(viewSpace.top / Sheet::Tile_Size));
	sf::Vector2i tileEnd(ceil((viewSpace.left + viewSpace.width) / Sheet::Tile_Size),
		ceil((viewSpace.top + viewSpace.height) / Sheet::Tile_Size));

	auto count{ 0u };
	for (int x = tileBegin.x; x <= tileEnd.x; ++x) 
	{
		for (int y = tileBegin.y; y <= tileEnd.y; ++y) 
		{
			if (x < 0 || y < 0) 
				continue; 

			auto tile{ GetTile(x, y) };
			if (!tile) 
				continue; 

			auto& sprite{ tile->m_properties->m_sprite };
			sprite.setPosition(x * Sheet::Tile_Size, y * Sheet::Tile_Size);
			wind->draw(sprite);
			++count;


			if (m_context->m_debugOverlay.Debug()) 
			{
				if (tile->m_properties->m_deadly || tile->m_warp) 
				{
					auto tileMarker{ std::make_unique<sf::RectangleShape>(sf::Vector2f(Sheet::Tile_Size, Sheet::Tile_Size)) };
					tileMarker->setPosition(x * Sheet::Tile_Size, y * Sheet::Tile_Size);
					if (tile->m_properties->m_deadly) 
						tileMarker->setFillColor(sf::Color(255, 0, 0, 100));
					
					else if (tile->m_warp) 
						tileMarker->setFillColor(sf::Color(0, 255, 0, 150));
					
					m_context->m_debugOverlay.Add(std::move(tileMarker));
				}
			}
		
		}
	}
}

void Map::PurgeMap() 
{
	m_tileCount = 0u;
	m_tileMap.clear();
	m_context->m_entityManager->Purge();

	if (m_backgroundTexture == "") 
		return; 

	m_context->m_textureManager->ReleaseResource(m_backgroundTexture);
	m_backgroundTexture = "";
}

void Map::PurgeTileSet()
{
	m_tileSet.clear();
	m_tileSetCount = 0u;
}