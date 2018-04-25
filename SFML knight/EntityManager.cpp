#include "EntityManager.hpp"
#include "SharedContext.hpp"

EntityManager::EntityManager(SharedContext* context, unsigned int maxEntities)
	:m_context(context), m_maxEntities(maxEntities)
{
	LoadEnemyTypes("EnemyList.list");
	RegisterEntity<Player>(EntityType::Player);
	RegisterEntity<Enemy>(EntityType::Enemy);
}

int EntityManager::Add(const EntityType& type, const std::string& name)
{
	auto itr{ m_entityFactory.find(type) };
	if (itr == m_entityFactory.end())  
		return -1; 

	auto entity{ itr->second() };
	auto reserve{ entity.get() };
	entity->m_id = m_idCounter;
	if (name != "")  
		entity->m_name = name; 

	m_entities.emplace(m_idCounter,std::move(entity));

	if (type == EntityType::Enemy) 
	{
		auto itr{ m_enemyTypes.find(name) };
		if (itr != m_enemyTypes.end()) 
		{
			auto enemy{ static_cast<Enemy*>(reserve) };
			enemy->Load(itr->second);
		}
	}

	++m_idCounter;
	return m_idCounter - 1;
}

EntityBase* EntityManager::Find(const std::string& name)
{
	for (auto &itr : m_entities) 
		if (itr.second->GetName() == name) 
			return itr.second.get();
	
	return nullptr;
}

EntityBase* EntityManager::Find(unsigned int id) 
{
	auto itr{ m_entities.find(id) };
	if (itr == m_entities.end()) 
		return nullptr; 

	return itr->second.get();
}


void EntityManager::Update(float dT) 
{
	for (auto &itr : m_entities) 
		itr.second->Update(dT);

	EntityCollisionCheck();
	ProcessRemovals();
}

void EntityManager::Draw() 
{
	auto window{ m_context->m_wind->GetRenderWindow() };
	auto viewSpace{ m_context->m_wind->GetViewSpace() };

	for (auto &itr : m_entities) 
	{
		if (!viewSpace.intersects(itr.second->m_AABB))  
			continue; 

		itr.second->Draw(window);
	}
}

void EntityManager::Purge() 
{
	m_entities.clear();
	m_idCounter = 0;
}

void EntityManager::ProcessRemovals() 
{
	while (m_entitiesToRemove.begin() != m_entitiesToRemove.end()) 
	{
		auto id{ m_entitiesToRemove.back() };
		auto itr{ m_entities.find(id) };
		if (itr != m_entities.end()) 
		{
			std::cout << "Discarding entity: " << itr->second->GetId() << std::endl;
			m_entities.erase(itr);
		}
		m_entitiesToRemove.pop_back();
	}
}

void EntityManager::EntityCollisionCheck() 
{
	if (m_entities.empty()) 
		return; 

	for (auto itr{ m_entities.begin() }; std::next(itr) != m_entities.end(); ++itr)
	{
		for (auto itr2{ std::next(itr) }; itr2 != m_entities.end(); ++itr2)
		{
			if (itr->first == itr2->first)  
				continue; 

			// Regular AABB bounding box collision.
			if (itr->second->m_AABB.intersects(itr2->second->m_AABB)) 
			{
				itr->second->OnEntityCollision(itr2->second.get(), false);
				itr2->second->OnEntityCollision(itr->second.get(), false);
			}

			auto t1{ itr->second->GetType() };
			auto t2{ itr2->second->GetType() };
			if (t1 == EntityType::Player || t1 == EntityType::Enemy) {
				auto c1{ static_cast<Character*>(itr->second.get()) };
				if (c1->m_attackAABB.intersects(itr2->second->m_AABB)) {
					c1->OnEntityCollision(itr2->second.get(), true);
				}
			}

			if (t2 == EntityType::Player || t2 == EntityType::Enemy) {
				auto c2{ static_cast<Character*>(itr2->second.get()) };
				if (c2->m_attackAABB.intersects(itr->second->m_AABB)) {
					c2->OnEntityCollision(itr->second.get(), true);
				}
			}
		}
	}
}

void EntityManager::LoadEnemyTypes(const std::string& name)
{
	std::ifstream file(Utils::GetWorkingDirectory() + std::string("/Resources/media/Characters/") + name);
	if (!file) 
	{ 
		std::cout << "Failed loading file: " << name << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line)) 
	{
		if (line[0] == '|') 
			continue; 

		std::stringstream keystream(line);
		std::string name;
		std::string charFile;
		keystream >> name >> charFile;
		m_enemyTypes.emplace(name, charFile);
	}
}
