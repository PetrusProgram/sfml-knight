#pragma once
#include "sfml\Graphics.hpp"
#include <vector>
#include <memory>


class DebugOverlay 
{
public:

	void Add(std::unique_ptr<sf::Drawable> drawable)
	{
		m_drawables.push_back(std::move(drawable));
	}

	void Draw(sf::RenderWindow* wind);

	bool Debug() { return m_debug; }
	void SetDebug(bool val) { m_debug = val; }
private:
	std::vector<std::unique_ptr<sf::Drawable>> m_drawables;
	bool m_debug{ false };
};