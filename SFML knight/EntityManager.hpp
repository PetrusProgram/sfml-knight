#pragma once
#include <functional>
#include <unordered_map>
#include "Player.hpp"
#include "Enemy.hpp"


using EntityContainer = std::unordered_map<unsigned int, std::unique_ptr<EntityBase>>;
using EntityFactory = std::unordered_map<EntityType, std::function<std::unique_ptr<EntityBase>()>>;
using EnemyTypes = std::unordered_map<std::string, std::string>;

struct SharedContext;

class EntityManager
{
public:
	EntityManager() = delete;
	EntityManager(SharedContext* context, unsigned int maxEntities);
	

	int Add(const EntityType& type, const std::string& name = "");
	EntityBase* Find(unsigned int id);
	EntityBase* Find(const std::string& name);
	void Remove(unsigned int id) { m_entitiesToRemove.emplace_back(id); }

	void Update(float dT);
	void Draw();

	void Purge();

	SharedContext* GetContext() { return m_context; }
private:
	template<class T>
	void RegisterEntity(const EntityType& type) {
		m_entityFactory[type] = [this]()
		{
			return std::make_unique<T>(this);
		};
	}

	void ProcessRemovals();
	void LoadEnemyTypes(const std::string& name);
	void EntityCollisionCheck();

	EntityContainer m_entities;
	EnemyTypes m_enemyTypes;
	EntityFactory m_entityFactory;
	SharedContext* m_context{ nullptr };
	unsigned int m_idCounter{ 0u };
	unsigned int m_maxEntities{ 0u };

	std::vector<unsigned int> m_entitiesToRemove;
};