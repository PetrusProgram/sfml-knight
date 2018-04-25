#include "IntroState.hpp"
#include "StateManager.hpp"


void IntroState::OnCreate() 
{
	auto windowSize{ m_stateMgr->GetContext()->m_wind->GetRenderWindow()->getSize() };

	auto textureMgr{ m_stateMgr->GetContext()->m_textureManager };
	textureMgr->RequireResource("Intro");
	m_introSprite.setTexture(*textureMgr->GetResource("Intro"));
	m_introSprite.setOrigin(textureMgr->GetResource("Intro")->getSize().x / 2.0f,
		textureMgr->GetResource("Intro")->getSize().y / 2.0f);

	m_introSprite.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	m_font.loadFromFile(Utils::GetWorkingDirectory() + "/Resources/media/Fonts/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString(sf::String("Press SPACE to continue"));
	m_text.setCharacterSize(15);
	auto textRect{ m_text.getLocalBounds() };
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	m_text.setPosition(m_introSprite.getPosition().x,
		m_introSprite.getPosition().y + textureMgr->GetResource("Intro")->getSize().y / 1.5f);

	auto evMgr{ m_stateMgr->GetContext()->m_eventManager };
	evMgr->AddCallback(StateType::Intro, "Intro_Continue", &IntroState::Continue, this);
}

void IntroState::OnDestroy() 
{
	auto textureMgr{ m_stateMgr->GetContext()->m_textureManager };
	textureMgr->ReleaseResource("Intro");

	auto evMgr{ m_stateMgr->GetContext()->m_eventManager };
	evMgr->RemoveCallback(StateType::Intro, "Intro_Continue");
}

void IntroState::Draw() 
{
	auto window{ m_stateMgr->GetContext()->m_wind->GetRenderWindow() };

	window->draw(m_introSprite);
	window->draw(m_text);
}

void IntroState::Continue(EventDetails* details) 
{
	m_stateMgr->SwitchTo(StateType::Menu);
	m_stateMgr->Remove(StateType::Intro);
}

void IntroState::Update(const sf::Time& time) {}
void IntroState::Activate() {}
void IntroState::Deactivate() {}