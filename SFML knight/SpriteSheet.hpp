#pragma once
#include "SFML\Graphics.hpp"
#include "TextureManager.hpp"
#include "Utilities.hpp"
#include "AnimationDirection.hpp"
#include <unordered_map>
using Animations = std::unordered_map<std::string, std::unique_ptr<AnimationBase>>;

class SpriteSheet 
{
public:
	SpriteSheet(TextureManager* textMgr):m_textureManager(textMgr) {}
	~SpriteSheet() { ReleaseSheet(); }

	void CropSprite(const sf::IntRect& rect) { m_sprite.setTextureRect(rect); }
	sf::Vector2i GetSpriteSize()const { return m_spriteSize; }
	sf::Vector2f GetSpritePosition()const { return m_sprite.getPosition(); }
	void SetSpriteSize(const sf::Vector2i& size);
	void SetSpritePosition(const sf::Vector2f& pos) { m_sprite.setPosition(pos); }

	void SetDirection(const Direction& dir);
	Direction GetDirection()const { return m_direction; }

	bool LoadSheet(const std::string& file);
	void ReleaseSheet();

	AnimationBase* GetCurrentAnim() { return m_animationCurrent; }
	bool SetAnimation(const std::string_view name, bool play = false, bool loop = false);

	void Update(const float& dT) { m_animationCurrent->Update(dT); }
	void Draw(sf::RenderWindow* wnd) { wnd->draw(m_sprite); }
private:
	std::string m_texture;
	sf::Sprite m_sprite;
	sf::Vector2i m_spriteSize;
	sf::Vector2f m_spriteScale{ 1.f, 1.f };
	Direction m_direction{ Direction::Right };

	std::string m_animType;
	Animations m_animations;
	AnimationBase* m_animationCurrent{ nullptr };

	TextureManager* m_textureManager{ nullptr };
};