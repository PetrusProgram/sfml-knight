#include "Player.hpp"
#include "StateManager.hpp"
#include "EntityManager.hpp"


Player::Player(EntityManager* entityMgr) : Character(entityMgr)
{
	Load("Player.char");
	m_type = EntityType::Player;

	auto events{ m_entityManager->GetContext()->m_eventManager };
	events->AddCallback<Player>(StateType::Game, "Player_MoveLeft", &Player::React, this);
	events->AddCallback<Player>(StateType::Game, "Player_MoveRight", &Player::React, this);
	events->AddCallback<Player>(StateType::Game, "Player_Jump", &Player::React, this);
	events->AddCallback<Player>(StateType::Game, "Player_Attack", &Player::React, this);
}

Player::~Player() 
{
	auto events{ m_entityManager->GetContext()->m_eventManager };
	events->RemoveCallback(StateType::Game, "Player_MoveLeft");
	events->RemoveCallback(StateType::Game, "Player_MoveRight");
	events->RemoveCallback(StateType::Game, "Player_Jump");
	events->RemoveCallback(StateType::Game, "Player_Attack");
}

void Player::React(EventDetails* details)
{
	if (details->m_name == "Player_MoveLeft") 
		Move(Direction::Left);
	
	else if (details->m_name == "Player_MoveRight") 
		Move(Direction::Right);
	
	else if (details->m_name == "Player_Jump") 
		Jump();
	
	else if (details->m_name == "Player_Attack") 
		Attack();
	
}

void Player::OnEntityCollision(EntityBase* collider, bool attack) 
{
	if (m_state == EntityState::Dying) 
		return; 

	if (attack)
	{
		if (m_state != EntityState::Attacking)  
			return; 

		if (!m_spriteSheet.GetCurrentAnim()->IsInAction())  
			return; 

		if (collider->GetType() != EntityType::Enemy && collider->GetType() != EntityType::Player)
			return;
		
		Character* opponent = (Character*)collider;
		opponent->GetHurt(1);
		if (m_position.x > opponent->GetPosition().x) 
			opponent->AddVelocity(-32.f, 0.f);
		
		else 
			opponent->AddVelocity(32.f, 0.f);
		
	}
}