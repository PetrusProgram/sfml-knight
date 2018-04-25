#include "GameOverState.hpp"

void GameOverState::OnCreate()
{
	m_elapsed = 0.f;
	m_font.loadFromFile(Utils::GetWorkingDirectory() + "/Resources/media/Fonts/arial.ttf");
	m_text.setFont(m_font);
	m_text.setCharacterSize(16u);
	m_text.setString("You beat the game! Congratulations!");
	m_text.setFillColor(sf::Color::White);
	m_text.setOrigin(m_text.getLocalBounds().width / 2.f, m_text.getLocalBounds().height / 2.f);
	m_text.setPosition(400.f, 300.f);

	m_stateMgr->Remove(StateType::Game);
}
void GameOverState::Update(const sf::Time& time)
{
	m_elapsed += time.asSeconds();
	if (m_elapsed >= 5.0f)
	{
		m_stateMgr->Remove(StateType::GameOver);
		m_stateMgr->SwitchTo(StateType::Menu);
	}
}

void GameOverState::Draw()
{
	auto window{ m_stateMgr->GetContext()->m_wind->GetRenderWindow() };
	window->draw(m_text);
}