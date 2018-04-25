#include "GameState.hpp"
#include "StateManager.hpp"


void GameState::OnCreate()
{
	auto evMgr{ m_stateMgr->GetContext()->m_eventManager };

	evMgr->AddCallback(StateType::Game, "Key_Escape", &GameState::Menu, this);
	evMgr->AddCallback(StateType::Game, "Key_P", &GameState::Pause, this);
	evMgr->AddCallback(StateType::Game, "Key_O", &GameState::ToggleOverlay, this);

	auto size{ m_stateMgr->GetContext()->m_wind->GetWindowSize() };
	m_view.setSize(size.x, size.y);
	m_view.setCenter(size.x / 2.f, size.y / 2.f);
	m_view.zoom(0.6f);
	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_view);

	m_gameMap = std::make_unique<Map>(m_stateMgr->GetContext(), this);
	m_gameMap->LoadMap("/Resources/media/Maps/map1.map");
}

void GameState::OnDestroy() 
{
	auto evMgr{ m_stateMgr->GetContext()->m_eventManager };
	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_P");
	evMgr->RemoveCallback(StateType::Game, "Key_O");
	m_gameMap.reset();
}

void GameState::Update(const sf::Time& time) 
{
	auto context{ m_stateMgr->GetContext() };
	auto player{ context->m_entityManager->Find("Player") };
	if (!player) 
	{
		std::cout << "Respawning player..." << std::endl;
		context->m_entityManager->Add(EntityType::Player, "Player");
		player = context->m_entityManager->Find("Player");
		player->SetPosition(m_gameMap->GetPlayerStart());
	}
	else 
	{
		m_view.setCenter(player->GetPosition());
		context->m_wind->GetRenderWindow()->setView(m_view);
	}

	auto viewSpace{ context->m_wind->GetViewSpace() };
	if (viewSpace.left <= 0.f) 
	{
		m_view.setCenter(viewSpace.width / 2.f, m_view.getCenter().y);
		context->m_wind->GetRenderWindow()->setView(m_view);
	}
	else if (viewSpace.left + viewSpace.width > (m_gameMap->GetMapSize().x + 1.f) * Sheet::Tile_Size) 
	{
		m_view.setCenter(((m_gameMap->GetMapSize().x + 1.f) * Sheet::Tile_Size) - (viewSpace.width / 2.f), m_view.getCenter().y);
		context->m_wind->GetRenderWindow()->setView(m_view);
	}

	m_gameMap->Update(time.asSeconds());
	m_stateMgr->GetContext()->m_entityManager->Update(time.asSeconds());
}

void GameState::Draw()
{
	m_gameMap->Draw();
	m_stateMgr->GetContext()->m_entityManager->Draw();
}
void GameState::Pause(EventDetails* details) { m_stateMgr->SwitchTo(StateType::Pause); }
void GameState::Menu(EventDetails* details) { m_stateMgr->SwitchTo(StateType::Menu); }
void GameState::ToggleOverlay(EventDetails* details)
{
	m_stateMgr->GetContext()->m_debugOverlay.SetDebug(!m_stateMgr->GetContext()->m_debugOverlay.Debug());
}