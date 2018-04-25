#include "Game.hpp"


Game::Game() 
{
	m_clock.restart();
	m_context.m_wind = &m_window;
	m_context.m_eventManager = m_window.GetEventManager();
	m_context.m_textureManager = &m_textureManager;
	m_context.m_entityManager = &m_entityManager;
	m_stateManager.SwitchTo(StateType::Intro);
}


void Game::Update() 
{
		m_window.Update();
		m_stateManager.Update(m_elapsed);
}

void Game::Render()
{
	m_window.BeginDraw();
	m_stateManager.Draw();

	if (m_context.m_debugOverlay.Debug()) 
		m_context.m_debugOverlay.Draw(m_window.GetRenderWindow());

	m_window.EndDraw();
}

void Game::LateUpdate() 
{
	m_stateManager.ProcessRequests();
	RestartClock();
}