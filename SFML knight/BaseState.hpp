#pragma once
#include "sfml\Graphics.hpp"


class BaseState
{
	friend class StateManager;
public:
	BaseState(StateManager* stateManager) : m_stateMgr(stateManager)  {}
	virtual ~BaseState() = default;

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& time) = 0;
	virtual void Draw() = 0;

	void SetTransparent(bool transparent) { m_transparent = transparent; }
	bool IsTransparent()const { return m_transparent; }
	void SetTranscendent(bool transcendence) { m_transcendent = transcendence; }
	bool IsTranscendent()const { return m_transcendent; }
	sf::View& GetView() { return m_view; }
	StateManager* GetStateManager() { return m_stateMgr; }
protected:
	StateManager * m_stateMgr{ nullptr };
	bool m_transparent{ false };
	bool m_transcendent{ false };
	sf::View m_view;
};