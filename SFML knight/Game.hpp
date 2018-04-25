#pragma once
#include "Window.hpp"
#include "EventManager.hpp"
#include "StateManager.hpp"
#include "TextureManager.hpp"
#include "EntityManager.hpp"
#include <iostream>
class Game 
{
public:
	Game();

	void Update();
	void Render();
	void LateUpdate();

	sf::Time GetElapsed() { return m_clock.getElapsedTime(); }

	Window* GetWindow() { return &m_window; }
private:
	void RestartClock() { m_elapsed = m_clock.restart(); }
	inline static const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
	sf::Clock m_clock;
	sf::Time m_elapsed;
	SharedContext m_context;
	Window m_window{ "SFML knight", sf::Vector2u(800, 600) };
	EntityManager m_entityManager{ &m_context, 100 };
	TextureManager m_textureManager;
	StateManager m_stateManager{ &m_context };
};