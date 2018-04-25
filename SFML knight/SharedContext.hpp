#pragma once
#include "Window.hpp"
#include "EventManager.hpp"
#include "TextureManager.hpp"
#include "EntityManager.hpp"
#include "DebugOverlay.hpp"'

class Map;
struct SharedContext 
{
	SharedContext() = default;
	Window* m_wind{ nullptr };
	EventManager* m_eventManager{ nullptr };
	TextureManager* m_textureManager{ nullptr };
	EntityManager* m_entityManager{ nullptr };
	Map* m_gameMap{ nullptr };
	DebugOverlay m_debugOverlay;
};