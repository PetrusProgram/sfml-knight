#pragma once
#include "SpriteSheet.hpp"
#include "EntityBase.hpp"

class Character : public EntityBase 
{
	friend class EntityManager;
public:
	Character(EntityManager* entityMgr);
	virtual ~Character() = default;

	void Move(Direction dir);

	void Jump();
	void Attack();
	void GetHurt(int damage);

	void Load(const std::string& path);

	virtual void OnEntityCollision(EntityBase* collider, bool attack) = 0;

	virtual void Update(float dT);
	void Draw(sf::RenderWindow* wind){ m_spriteSheet.Draw(wind); }
protected:
	void UpdateAttackAABB();
	void Animate();

	SpriteSheet m_spriteSheet;
	float m_jumpVelocity{ 250.f };
	int m_hitpoints{ 5 };
	sf::FloatRect m_attackAABB;
	sf::Vector2f m_attackAABBoffset;
};