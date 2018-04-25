#include "MenuState.hpp"
#include "StateManager.hpp"




MenuState::~MenuState() { OnDestroy(); }

void MenuState::OnCreate() 
{
	m_font.loadFromFile(Utils::GetWorkingDirectory() + "/Resources/media/Fonts/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString(sf::String("MAIN MENU:"));
	m_text.setCharacterSize(18);

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,	textRect.top + textRect.height / 2.0f);

	m_text.setPosition(400, 100);

	m_buttonSize = sf::Vector2f(300.0f, 32.0f);
	m_buttonPos = sf::Vector2f(400, 200);
	m_buttonPadding = 4; 
	std::array<std::string, 2> str= { "PLAY", "EXIT" };


	for (int i = 0; i < 2; ++i) 
	{
		sf::Vector2f buttonPosition(m_buttonPos.x, m_buttonPos.y + (i * (m_buttonSize.y + m_buttonPadding)));
		m_rects.at(i).setSize(m_buttonSize);
		m_rects.at(i).setFillColor(sf::Color::Red);

		m_rects.at(i).setOrigin(m_buttonSize.x / 2.0f, m_buttonSize.y / 2.0f);
		m_rects.at(i).setPosition(buttonPosition);

		m_labels.at(i).setFont(m_font);
		m_labels.at(i).setString(static_cast<sf::String>(str.at(i)));
		m_labels.at(i).setCharacterSize(12u);

		auto rect{ m_labels.at(i).getLocalBounds() };
		m_labels.at(i).setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
		m_labels.at(i).setPosition(buttonPosition);
	}

	auto evMgr{ m_stateMgr->GetContext()->m_eventManager };
	evMgr->AddCallback(StateType::Menu, "Mouse_Left", &MenuState::MouseClick, this);
}

void MenuState::OnDestroy() 
{
	auto evMgr{ m_stateMgr->GetContext()->m_eventManager };
	evMgr->RemoveCallback(StateType::Menu, "Mouse_Left");
}

void MenuState::Activate() {
	if (m_stateMgr->HasState(StateType::Game) && m_labels[0].getString() != "RESUME")
		m_labels[0].setString(sf::String("RESUME"));
	
	else 
		m_labels[0].setString(sf::String("PLAY"));
	

	auto rect{ m_labels[0].getLocalBounds() };
	m_labels[0].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
}

void MenuState::MouseClick(EventDetails* details)
{
	auto context{ m_stateMgr->GetContext() };
	auto mousePos{ details->m_mouse };

	auto halfX{ m_buttonSize.x / 2.0f };
	auto halfY{ m_buttonSize.y / 2.0f };
	for (int i = 0; i < 2; ++i) 
	{
		if (mousePos.x >= m_rects.at(i).getPosition().x - halfX &&	mousePos.x <= m_rects.at(i).getPosition().x + halfX &&
			mousePos.y >= m_rects.at(i).getPosition().y - halfY &&	mousePos.y <= m_rects.at(i).getPosition().y + halfY)
		{
			if (i == 0) 
				m_stateMgr->SwitchTo(StateType::Game);
			
			else 
				m_stateMgr->GetContext()->m_wind->Close();
		}
	}
}

void MenuState::Draw() 
{
	auto window{ m_stateMgr->GetContext()->m_wind->GetRenderWindow() };
	window->draw(m_text);
	for (int i = 0; i < 2; ++i) 
	{
		window->draw(m_rects.at(i));
		window->draw(m_labels.at(i));
	}
}