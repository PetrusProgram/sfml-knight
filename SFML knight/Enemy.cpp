#include "Enemy.hpp"
#include <random>

void Enemy::OnEntityCollision(EntityBase* collider, bool attack)
{
	if (m_state == EntityState::Dying)  
		return; 

	if (attack)
		return;
	
	if (collider->GetType() != EntityType::Player) 
		return; 

	auto player{ static_cast<Character*>(collider) };
	SetState(EntityState::Attacking);
	player->GetHurt(1);
	if (m_position.x > player->GetPosition().x) 
	{
		player->AddVelocity(-m_speed.x, 0.f);
		m_spriteSheet.SetDirection(Direction::Left);
	}
	else 
	{
		player->AddVelocity(m_speed.y, 0.f);
		m_spriteSheet.SetDirection(Direction::Right);
	}
}

void Enemy::Update(float dT)
{
	Character::Update(dT);

	if (m_hasDestination) 
	{
		if (abs(m_destination.x - m_position.x) < 16.f) 
		{ 
			m_hasDestination = false;
			return; 
		}
		if (m_destination.x - m_position.x > 0.f)
			Move(Direction::Right); 

		else
			Move(Direction::Left); 

		if (m_collidingOnX)  
			m_hasDestination = false; 

		return;
	}
	static std::random_device r;
	static std::mt19937 RandomEngine(r());
	std::uniform_int_distribution<int> distr(1, 1000);

	int random{ distr(RandomEngine) };
	if (random != 1000) 
		return; 

	
	int newX{ distr(RandomEngine)%65 };
	if (distr(RandomEngine) % 2) 
		newX = -newX; 

	m_destination.x = m_position.x + newX;
	if (m_destination.x < 0.f)  
		m_destination.x = 0.f; 

	m_hasDestination = true;
}