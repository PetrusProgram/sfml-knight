#pragma once
#include "BaseState.hpp"
#include "Map.hpp"
#include "EventManager.hpp"

class GameState : public BaseState {
public:
	GameState(StateManager* stateManager) : BaseState(stateManager) {}


	void OnCreate();
	void OnDestroy();

	void Activate() {}
	void Deactivate() {}

	void Update(const sf::Time& time);
	void Draw();

	void Menu(EventDetails* details);
	void Pause(EventDetails* details);
	void ToggleOverlay(EventDetails* details);
private:
	std::unique_ptr<Map> m_gameMap;
};