#include "SpriteSheet.hpp"


void SpriteSheet::ReleaseSheet()
{
	m_textureManager->ReleaseResource(m_texture);
	m_animationCurrent = nullptr;
	m_animations.clear();
}

void SpriteSheet::SetSpriteSize(const sf::Vector2i& size) 
{
	m_spriteSize = size;
	m_sprite.setOrigin(m_spriteSize.x / 2, m_spriteSize.y);
}


void SpriteSheet::SetDirection(const Direction& dir) 
{
	if (dir == m_direction) 
		return; 

	m_direction = dir;
	m_animationCurrent->CropSprite();
}


bool SpriteSheet::LoadSheet(const std::string& file)
{
	std::ifstream sheet(Utils::GetWorkingDirectory() + file);
	if (sheet) 
	{
		ReleaseSheet(); 
		std::string line;
		while (std::getline(sheet, line)) 
		{
			if (line[0] == '|') 
				continue; 

			std::stringstream keystream(line);
			std::string type;
			keystream >> type;
			if (type == "Texture")
			{
				if (m_texture != "") 
				{
					std::cerr << "Duplicate texture entries in: " << file << std::endl;
					continue;
				}
				std::string texture;
				keystream >> texture;
				if (!m_textureManager->RequireResource(texture))
				{
					std::cerr << "Could not set up the texture: " << texture << std::endl;
					continue;
				}
				m_texture = texture;
				m_sprite.setTexture(*m_textureManager->GetResource(m_texture));
			}
			else if (type == "Size") 
			{
				keystream >> m_spriteSize.x >> m_spriteSize.y;
				SetSpriteSize(m_spriteSize);
			}
			else if (type == "Scale") 
			{
				keystream >> m_spriteScale.x >> m_spriteScale.y;
				m_sprite.setScale(m_spriteScale);
			}
			else if (type == "AnimationType") 
				keystream >> m_animType;
			
			else if (type == "Animation") 
			{
				std::string name;
				keystream >> name;
				if (m_animations.find(name) != m_animations.end())
				{
					std::cerr << "Duplicate animation(" << name << ") in: " << file << std::endl;
					continue;
				}
				std::unique_ptr<AnimationBase> anim;
				if (m_animType == "Directional") 
					anim = std::make_unique<AnimationDirection>();
				
				else 
				{
					std::cerr << "! Unknown animation type: " << m_animType << std::endl;
					continue;
				}

				keystream >> *anim;
				anim->SetSpriteSheet(this);
				anim->SetName(name);
				anim->Reset();
				auto animReserve{ anim.get() };
				m_animations.emplace(std::move(name), std::move(anim));

				if (m_animationCurrent)  
					continue; 

				m_animationCurrent = animReserve;
				m_animationCurrent->Play();
			}
		}
		return true;
	}
	std::cerr << "Failed loading spritesheet: " << file << std::endl;
	return false;
}

bool SpriteSheet::SetAnimation(const std::string_view name, bool play, bool loop)
{
	auto itr{ m_animations.find(name.data()) };
	if (itr == m_animations.end())  
		return false; 

	if (itr->second.get() == m_animationCurrent) 
		return false; 

	if (m_animationCurrent)
		m_animationCurrent->Stop(); 

	m_animationCurrent = itr->second.get();
	m_animationCurrent->SetLooping(loop);
	if (play) 
		m_animationCurrent->Play(); 

	m_animationCurrent->CropSprite();
	return true;
}