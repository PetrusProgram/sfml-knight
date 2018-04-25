#pragma once
#include "IntroState.hpp"
#include "GameOverState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "MenuState.hpp"
#include "SharedContext.hpp"
#include <unordered_map>
#include <vector>
#include <memory>

enum class StateType
{
	Intro = 1,
	Menu, 
	Game,
	Pause, 
	GameOver,
	Credits 
};

using StateContainer = std::vector<std::pair<StateType, std::unique_ptr<BaseState>>>;
using TypeContainer = std::vector<StateType>;
using StateFactory = std::unordered_map<StateType, std::function<std::unique_ptr<BaseState>()>>;

class StateManager
{
public:
	StateManager(SharedContext* shared);
	~StateManager();

	void Update(const sf::Time& time);
	void Draw();

	void ProcessRequests();

	SharedContext* GetContext() { return m_shared; }
	bool HasState(const StateType& type);

	void SwitchTo(const StateType& type);
	void Remove(const StateType& type) { m_toRemove.push_back(type); }
private:
	void CreateState(const StateType& type);
	void RemoveState(const StateType& type);

	template<class T>
	void RegisterState(const StateType& type) {
		m_stateFactory[type] = [this]() 
		{
			return std::make_unique<T>(this);
		};
	}

	SharedContext* m_shared{ nullptr };
	StateContainer m_states;
	TypeContainer m_toRemove;
	StateFactory m_stateFactory;
};