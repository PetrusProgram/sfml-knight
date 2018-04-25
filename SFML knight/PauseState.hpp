#pragma once
#include "BaseState.hpp"
#include "EventManager.hpp"


class PauseState : public BaseState 
{
public:
	PauseState(StateManager* stateManager) : BaseState(stateManager) {}
	~PauseState() { OnDestroy(); }

	void OnCreate();
	void OnDestroy();

	void Activate() {}
	void Deactivate() {}

	void Update(const sf::Time& time) {}
	void Draw();

	void Unpause(EventDetails* details);
private:
	sf::Font m_font;
	sf::Text m_text;
	sf::RectangleShape m_rect;
};