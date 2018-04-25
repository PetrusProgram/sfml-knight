#pragma once
#include "BaseState.hpp"
#include "EventManager.hpp"

class IntroState : public BaseState {
public:
	IntroState(StateManager* stateManager)	: BaseState(stateManager) {}
	~IntroState() { OnDestroy(); }
	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void Continue(EventDetails* details);
private:
	sf::Sprite m_introSprite;
	sf::Font m_font;
	sf::Text m_text;
};