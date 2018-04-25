#pragma once
#include "BaseState.hpp"
#include "StateManager.hpp"

class GameOverState : public BaseState 
{
public:
	GameOverState(StateManager* stateManager) : BaseState(stateManager) {}

	void OnCreate();
	void OnDestroy() {}

	void Activate() {}
	void Deactivate() {}

	void Update(const sf::Time& time);
	void Draw();
private:
	sf::Font m_font;
	sf::Text m_text;
	float m_elapsed{ 0.f };
};