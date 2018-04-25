#pragma once
#include "EntityManager.hpp"
#include "Character.hpp"
#include "EventManager.hpp"
class Player : public Character
{
public:
	Player(EntityManager* entityMgr);
	~Player();

	void OnEntityCollision(EntityBase* collider, bool attack);
	void React(EventDetails* details);
};



