#include "PauseState.hpp"
#include "StateManager.hpp"


void PauseState::OnCreate()
{
	SetTransparent(true); 
	m_font.loadFromFile(Utils::GetWorkingDirectory() + "/Resources/media/Fonts/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString(sf::String("PAUSED"));
	m_text.setCharacterSize(14);
	m_text.setStyle(sf::Text::Bold);

	auto windowSize{ m_stateMgr->GetContext()->m_wind->GetRenderWindow()->getSize() };

	auto textRect{ m_text.getLocalBounds() };
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,	textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	m_rect.setSize(sf::Vector2f(windowSize));
	m_rect.setPosition(0, 0);
	m_rect.setFillColor(sf::Color(0, 0, 0, 150));

	auto evMgr{ m_stateMgr->GetContext()->m_eventManager };
	evMgr->AddCallback(StateType::Pause, "Key_P", &PauseState::Unpause, this);
}

void PauseState::OnDestroy()
{
	auto evMgr{ m_stateMgr->GetContext()->m_eventManager };
	evMgr->RemoveCallback(StateType::Pause, "Key_P");
}

void PauseState::Draw() 
{
	auto wind{ m_stateMgr->GetContext()->m_wind->GetRenderWindow() };
	wind->draw(m_rect);
	wind->draw(m_text);
}
void PauseState::Unpause(EventDetails* details) { m_stateMgr->SwitchTo(StateType::Game); }