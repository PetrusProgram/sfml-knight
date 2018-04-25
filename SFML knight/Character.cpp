#include "Character.hpp"
#include "StateManager.hpp"
#include "EntityManager.hpp"
#include <memory>

Character::Character(EntityManager* entityMgr) : EntityBase(entityMgr), m_spriteSheet(entityMgr->GetContext()->m_textureManager)
{
	m_name = "Character";
}

void Character::Move(Direction dir)
{
	if (GetState() == EntityState::Dying)  
		return; 

	m_spriteSheet.SetDirection(dir);
	if (dir == Direction::Left) 
		Accelerate(-m_speed.x, 0); 

	else 
		Accelerate(m_speed.x, 0); 

	if (GetState() == EntityState::Idle)  
		SetState(EntityState::Walking); 
}

void Character::Jump() 
{
	if (GetState() == EntityState::Dying || GetState() == EntityState::Jumping || GetState() == EntityState::Hurt)
		return; 

	SetState(EntityState::Jumping);
	AddVelocity(0, -m_jumpVelocity);
}

void Character::Attack() {
	if (GetState() == EntityState::Dying || GetState() == EntityState::Jumping ||
		GetState() == EntityState::Hurt || GetState() == EntityState::Attacking)
		return;

	SetState(EntityState::Attacking);
}

void Character::GetHurt(int damage) 
{
	if (GetState() == EntityState::Dying || GetState() == EntityState::Hurt)  
		return; 

	m_hitpoints = (m_hitpoints - damage > 0 ? m_hitpoints - damage : 0);
	if (m_hitpoints) 
		SetState(EntityState::Hurt); 

	else 
		SetState(EntityState::Dying); 
}

void Character::Load(const std::string& path) 
{
	std::ifstream file(Utils::GetWorkingDirectory() + std::string("/Resources/media/Characters/") + path);
	if (!file) 
	{ 
		std::cout << "Failed loading the character file: " << path << std::endl; 
		return; 
	}
	std::string line;
	while (std::getline(file, line)) {
		if (line[0] == '|') 
			continue; 

		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "Name") 
			keystream >> m_name;
		
		else if (type == "Spritesheet") 
		{
			std::string path;
			keystream >> path;
			m_spriteSheet.LoadSheet("/Resources/media/SpriteSheets/" + path);
		}
		else if (type == "Hitpoints") 
			keystream >> m_hitpoints;
		
		else if (type == "BoundingBox") 
		{
			sf::Vector2f boundingSize;
			keystream >> boundingSize.x >> boundingSize.y;
			SetSize(boundingSize.x, boundingSize.y);
		}
		else if (type == "DamageBox") 
			keystream >> m_attackAABBoffset.x >> m_attackAABBoffset.y >> m_attackAABB.width >> m_attackAABB.height;
		
		else if (type == "Speed") 
			keystream >> m_speed.x >> m_speed.y;
		
		else if (type == "JumpVelocity") 
			keystream >> m_jumpVelocity;
		
		else if (type == "MaxVelocity") 
			keystream >> m_maxVelocity.x >> m_maxVelocity.y;
		
		else 
			std::cout << "Unknown type in character file: " << type << std::endl;
		
	}
}

void Character::UpdateAttackAABB()
{
	m_attackAABB.left =	(m_spriteSheet.GetDirection() == Direction::Left ?(m_AABB.left - m_attackAABB.width) - m_attackAABBoffset.x
			: (m_AABB.left + m_AABB.width) + m_attackAABBoffset.x);
	m_attackAABB.top = m_AABB.top + m_attackAABBoffset.y;
}

void Character::Animate()
{
	auto state{ GetState() };

	if (state == EntityState::Walking && m_spriteSheet.GetCurrentAnim()->GetName() != "Walk")
		m_spriteSheet.SetAnimation("Walk", true, true);
	
	else if (state == EntityState::Jumping && m_spriteSheet.GetCurrentAnim()->GetName() != "Jump")
		m_spriteSheet.SetAnimation("Jump", true, false);
	
	else if (state == EntityState::Attacking && m_spriteSheet.GetCurrentAnim()->GetName() != "Attack")
		m_spriteSheet.SetAnimation("Attack", true, false);

	else if (state == EntityState::Hurt && m_spriteSheet.GetCurrentAnim()->GetName() != "Hurt")
		m_spriteSheet.SetAnimation("Hurt", true, false);
	
	else if (state == EntityState::Dying && m_spriteSheet.GetCurrentAnim()->GetName() != "Death")
		m_spriteSheet.SetAnimation("Death", true, false);
	
	else if (state == EntityState::Idle && m_spriteSheet.GetCurrentAnim()->GetName() != "Idle")
		m_spriteSheet.SetAnimation("Idle", true, true);
}

void Character::Update(float dT)
{
	EntityBase::Update(dT);
	if (m_attackAABB.width != 0 && m_attackAABB.height != 0)
	{
		UpdateAttackAABB();

		if (m_entityManager->GetContext()->m_debugOverlay.Debug()) 
		{
			auto arect{ std::make_unique<sf::RectangleShape>(sf::Vector2f(m_attackAABB.width, m_attackAABB.height)) };
			arect->setPosition(m_attackAABB.left, m_attackAABB.top);
			arect->setFillColor(sf::Color(255, 0, 0,
				(m_state == EntityState::Attacking && m_spriteSheet.GetCurrentAnim()->IsInAction()
					? 200 : 100)));
			m_entityManager->GetContext()->m_debugOverlay.Add(std::move(arect));
		}
	}
	if (GetState() != EntityState::Dying && GetState() != EntityState::Attacking && GetState() != EntityState::Hurt)
	{
		if (abs(m_velocity.y) >= 0.001f) 
			SetState(EntityState::Jumping);
		
		else if (abs(m_velocity.x) >= 0.1f) 
			SetState(EntityState::Walking);
		
		else 
			SetState(EntityState::Idle);
	}
	else if (GetState() == EntityState::Attacking || GetState() == EntityState::Hurt)
 {
		if (!m_spriteSheet.GetCurrentAnim()->IsPlaying()) {
			SetState(EntityState::Idle);
		}
	}
	else if (GetState() == EntityState::Dying) {
		if (!m_spriteSheet.GetCurrentAnim()->IsPlaying()) {
			m_entityManager->Remove(m_id);
		}
	}

	Animate();

	m_spriteSheet.Update(dT);
	m_spriteSheet.SetSpritePosition(m_position);
}