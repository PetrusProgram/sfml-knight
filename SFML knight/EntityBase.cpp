#include "EntityBase.hpp"
#include "EntityManager.hpp"
#include "SharedContext.hpp"
#include "Map.hpp"

void EntityBase::SetPosition(float x, float y)
{
	m_position = sf::Vector2f(x, y);
	UpdateAABB();
}

void EntityBase::SetPosition(const sf::Vector2f& pos) 
{
	m_position = pos;
	UpdateAABB();
}

void EntityBase::SetSize(float x, float y) 
{
	m_size = sf::Vector2f(x, y);
	UpdateAABB();
}


void EntityBase::SetState(EntityState state) 
{
	if (m_state == EntityState::Dying)  
		return; 

	m_state = state;
}

void EntityBase::Move(float x, float y) 
{
	m_positionOld = m_position;
	m_position += sf::Vector2f(x, y);
	auto mapSize{ m_entityManager->GetContext()->m_gameMap->GetMapSize() };
	if (m_position.x < 0.f) 
		m_position.x = 0.f;
	
	else if (m_position.x >(mapSize.x + 1.f) * Sheet::Tile_Size) 
		m_position.x = (mapSize.x + 1.f) * Sheet::Tile_Size;
	

	if (m_position.y < 0.f) 
		m_position.y = 0.f;
	
	else if (m_position.y >(mapSize.y + 1.f) * Sheet::Tile_Size) 
	{
		m_position.y = (mapSize.y + 1.f) * Sheet::Tile_Size;
		SetState(EntityState::Dying);
	}

	UpdateAABB();
}

void EntityBase::AddVelocity(float x, float y)
{
	m_velocity += sf::Vector2f(x, y);
	if (abs(m_velocity.x) > m_maxVelocity.x) 
	{
		if (m_velocity.x < 0.f) 
			m_velocity.x = -m_maxVelocity.x; 

		else 
			m_velocity.x = m_maxVelocity.x; 
	}

	if (abs(m_velocity.y) > m_maxVelocity.y) 
	{
		if (m_velocity.y < 0.f) { m_velocity.y = -m_maxVelocity.y; }
		else { m_velocity.y = m_maxVelocity.y; }
	}
}


void EntityBase::ApplyFriction(float x, float y) 
{
	if (m_velocity.x != 0.f) 
	{
		if (std::abs(m_velocity.x) - std::abs(x) < 0.f) 
			m_velocity.x = 0.f; 

		else 
		{
			if (m_velocity.x < 0.f)  
				m_velocity.x += x; 

			else 
				m_velocity.x -= x; 
		}
	}

	if (m_velocity.y != 0.f) 
	{
		if (std::abs(m_velocity.y) - std::abs(y) < 0.f) 
			m_velocity.y = 0; 

		else 
		{
			if (m_velocity.y < 0.f) 
				m_velocity.y += y; 

			else 
				m_velocity.y -= y; 
		}
	}
}

void EntityBase::Update(float dT)
{
	auto map{ m_entityManager->GetContext()->m_gameMap };
	auto gravity{ map->GetGravity() };
	Accelerate(0.f, gravity);
	AddVelocity(m_acceleration.x * dT, m_acceleration.y * dT);
	SetAcceleration(0.0f, 0.0f);
	sf::Vector2f frictionValue;
	if (m_referenceTile) 
	{
		frictionValue = m_referenceTile->m_friction;
		if (m_referenceTile->m_deadly)  
			SetState(EntityState::Dying); 
	}
	else if (map->GetDefaultTile()) 
		frictionValue = map->GetDefaultTile()->m_friction;
	
	else 
		frictionValue = m_friction;
	

	float frictionX{ (m_speed.x * frictionValue.x) * dT };
	float frictionY{ (m_speed.y * frictionValue.y) * dT };
	ApplyFriction(frictionX, frictionY);
	sf::Vector2f deltaPos{ m_velocity * dT };
	Move(deltaPos.x, deltaPos.y);
	m_collidingOnX = false;
	m_collidingOnY = false;
	CheckCollisions();
	ResolveCollisions();
}

void EntityBase::CheckCollisions()
{
	auto gameMap{ m_entityManager->GetContext()->m_gameMap };
	auto tileSize{ gameMap->GetTileSize() };
	int fromX = std::floor(m_AABB.left / tileSize);
	int toX = std::floor((m_AABB.left + m_AABB.width) / tileSize);
	int fromY = std::floor(m_AABB.top / tileSize);
	int toY = std::floor((m_AABB.top + m_AABB.height) / tileSize);

	for (int x = fromX; x <= toX; ++x) 
	{
		for (int y = fromY; y <= toY; ++y)
		{
			auto tile{ gameMap->GetTile(x, y) };
			if (!tile) 
				continue; 

			sf::FloatRect tileBounds(x * tileSize, y * tileSize, tileSize, tileSize);
			sf::FloatRect intersection;
			m_AABB.intersects(tileBounds, intersection);
			float area{ intersection.width * intersection.height };

			CollisionElement e(area, tile->m_properties, tileBounds);
			m_collisions.emplace_back(e);
			if (tile->m_warp && m_type == EntityType::Player) 
				gameMap->LoadNext();
		}
	}
}

void EntityBase::ResolveCollisions()
{
	if (!m_collisions.empty()) 
	{
		std::sort(m_collisions.begin(), m_collisions.end(), 
			[](const CollisionElement& first, const CollisionElement& second) { return first.m_area > second.m_area; });
		auto gameMap{ m_entityManager->GetContext()->m_gameMap };
		auto tileSize{ gameMap->GetTileSize() };
		for (auto &itr : m_collisions)
		{
			if (!m_AABB.intersects(itr.m_tileBounds))  
				continue; 

			if (m_entityManager->GetContext()->m_debugOverlay.Debug()) 
			{
				sf::Vector2f tempPos(itr.m_tileBounds.left, itr.m_tileBounds.top);
				auto rect{ std::make_unique<sf::RectangleShape>(sf::Vector2f(tileSize, tileSize)) };
				rect->setPosition(tempPos);
				rect->setFillColor(sf::Color(255, 255, 0, 150));
				m_entityManager->GetContext()->m_debugOverlay.Add(std::move(rect));
			}
			
			float xDiff{ (m_AABB.left + (m_AABB.width / 2)) - (itr.m_tileBounds.left + (itr.m_tileBounds.width / 2)) };
			float yDiff{ (m_AABB.top + (m_AABB.height / 2)) - (itr.m_tileBounds.top + (itr.m_tileBounds.height / 2)) };
			float resolve{ 0.f };
			if (std::abs(xDiff) > std::abs(yDiff)) 
			{
				if (xDiff > 0) 
					resolve = (itr.m_tileBounds.left + tileSize) - m_AABB.left;
				
				else 
					resolve = -((m_AABB.left + m_AABB.width) - itr.m_tileBounds.left);
				
				Move(resolve, 0.f);
				m_velocity.x = 0.f;
				m_collidingOnX = true;
			}
			else
			{
				if (yDiff > 0.f) 
					resolve = (itr.m_tileBounds.top + tileSize) - m_AABB.top;
				
				else 
					resolve = -((m_AABB.top + m_AABB.height) - itr.m_tileBounds.top);
				
				Move(0.f, resolve);
				m_velocity.y = 0.f;
				if (m_collidingOnY) 
					continue; 

				m_referenceTile = itr.m_tile;
				m_collidingOnY = true;
			}
		}
		m_collisions.clear();
	}
	if (!m_collidingOnY)
		m_referenceTile = nullptr; 
}