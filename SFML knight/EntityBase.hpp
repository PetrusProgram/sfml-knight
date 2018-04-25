#pragma once
#include "sfml\Graphics.hpp"
#include <string>


enum class EntityType 
{
	Base, 
	Enemy, 
	Player 
};

enum class EntityState 
{ 
	Idle,
	Walking, 
	Jumping, 
	Attacking, 
	Hurt, 
	Dying 
};

struct TileInfo;

struct CollisionElement 
{
	CollisionElement(float area, TileInfo* info, const sf::FloatRect& bounds) : m_area(area), m_tile(info), m_tileBounds(bounds) {}
	float m_area{ 0.f };
	TileInfo* m_tile{ nullptr };
	sf::FloatRect m_tileBounds;
};

using Collisions = std::vector<CollisionElement>;

class EntityManager;
class EntityBase
{
	friend class EntityManager;
public:
	EntityBase(EntityManager* entityMgr) :m_entityManager(entityMgr)  {}
	virtual ~EntityBase() = default;

	const sf::Vector2f& GetPosition()const { return m_position; }
	const sf::Vector2f& GetSize()const { return m_size; }
	EntityState GetState()const { return m_state; }
	std::string GetName()const { return m_name; }
	unsigned int GetId()const { return m_id; }
	EntityType GetType()const { return m_type; }

	void SetPosition(float x, float y);
	void SetPosition(const sf::Vector2f& pos);
	void SetSize(float x, float y);
	void SetState(EntityState state);

	void Accelerate(float x, float y) { m_acceleration += sf::Vector2f(x, y); }
	void SetAcceleration(float x, float y) { m_acceleration = sf::Vector2f(x, y); }
	void Move(float x, float y);
	void AddVelocity(float x, float y);
	void ApplyFriction(float x, float y);

	virtual void Update(float dT);
	virtual void Draw(sf::RenderWindow* wind) = 0;
protected:

	void UpdateAABB() { m_AABB = sf::FloatRect(m_position.x - (m_size.x / 2), m_position.y - m_size.y, m_size.x, m_size.y); }
	void CheckCollisions();
	void ResolveCollisions();

	virtual void OnEntityCollision(EntityBase* collider, bool attack) = 0;

	std::string m_name{ "BaseEntity" };
	EntityType m_type{ EntityType::Base };
	unsigned int m_id{ 0 };
	sf::Vector2f m_position; 
	sf::Vector2f m_positionOld; 
	sf::Vector2f m_velocity; 
	sf::Vector2f m_maxVelocity; 
	sf::Vector2f m_speed; 
	sf::Vector2f m_acceleration; 
	sf::Vector2f m_friction; 
	TileInfo* m_referenceTile{ nullptr };
	sf::Vector2f m_size; 
	sf::FloatRect m_AABB; 
	EntityState m_state{ EntityState::Idle }; 
	bool m_collidingOnX{ false };
	bool m_collidingOnY{ false };
	Collisions m_collisions;
	EntityManager* m_entityManager{ nullptr };
};