#include "DebugOverlay.hpp"

void DebugOverlay::Draw(sf::RenderWindow* wind) {
	while (m_drawables.begin() != m_drawables.end()) 
	{
		wind->draw(*m_drawables.back());;
		m_drawables.pop_back();
	}
}