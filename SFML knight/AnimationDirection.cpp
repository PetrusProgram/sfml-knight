#include "AnimationDirection.hpp"
#include "SpriteSheet.hpp"


void AnimationDirection::CropSprite() {
	sf::IntRect rect(m_spriteSheet->GetSpriteSize().x * m_frameCurrent,
		m_spriteSheet->GetSpriteSize().y * (m_frameRow + static_cast<int>(m_spriteSheet->GetDirection())),
		m_spriteSheet->GetSpriteSize().x, m_spriteSheet->GetSpriteSize().y);
	m_spriteSheet->CropSprite(rect);
}

void AnimationDirection::FrameStep() {
	if (m_frameStart < m_frameEnd)  
		++m_frameCurrent; 

	else 
		--m_frameCurrent; 

	if ((m_frameStart < m_frameEnd && m_frameCurrent > m_frameEnd) ||
		(m_frameStart > m_frameEnd && m_frameCurrent < m_frameEnd))
	{
		if (m_loop) 
		{ 
			m_frameCurrent = m_frameStart;
			return; 
		}
		m_frameCurrent = m_frameEnd;
		Pause();
	}
}

void AnimationDirection::ReadIn(std::stringstream& stream) {
	stream >> m_frameStart >> m_frameEnd >> m_frameRow
		>> m_frameTime >> m_frameActionStart >> m_frameActionEnd;
}