#pragma once
#include "BaseState.hpp"
#include "EventManager.hpp"
#include <array>

class MenuState : public BaseState {
public:
	MenuState(StateManager* stateManager) : BaseState(stateManager) {}
	~MenuState();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate() {}

	void Update(const sf::Time& time) {}
	void Draw();

	void MouseClick(EventDetails* details);
private:
	sf::Font m_font;
	sf::Text m_text;

	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	unsigned int m_buttonPadding{ 0u };

	std::array<sf::RectangleShape, 3> m_rects;
	std::array<sf::Text, 3> m_labels;
};