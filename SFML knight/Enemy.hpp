#pragma once
#include "Character.hpp"

class Enemy : public Character 
{
public:
	Enemy(EntityManager* entityMgr) :Character(entityMgr) {	m_type = EntityType::Enemy;	}

	void OnEntityCollision(EntityBase* collider, bool attack);
	void Update(float dT);
private:
	
	sf::Vector2f m_destination;
	bool m_hasDestination{ false };
};