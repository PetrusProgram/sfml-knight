#include "StateManager.hpp"


StateManager::StateManager(SharedContext* shared) : m_shared(shared)
{
	RegisterState<IntroState>(StateType::Intro);
	RegisterState<MenuState>(StateType::Menu);
	RegisterState<GameState>(StateType::Game);
	RegisterState<PauseState>(StateType::Pause);
	RegisterState<GameOverState>(StateType::GameOver);
}

StateManager::~StateManager() {
	for (auto &itr : m_states) 
		itr.second->OnDestroy();
}

void StateManager::Update(const sf::Time& time) 
{
	if (m_states.empty()) 
		return; 

	if (m_states.back().second->IsTranscendent() && m_states.size() > 1)
	{
		auto itr{ m_states.end() };
		while (itr != m_states.begin()) 
		{
			if (itr != m_states.end()) 
				if (!itr->second->IsTranscendent()) 
					break;
				
			--itr;
		}
		for (; itr != m_states.end(); ++itr) 
			itr->second->Update(time);
	}
	else 
		m_states.back().second->Update(time);
	
}

void StateManager::Draw() 
{
	if (m_states.empty()) 
		return; 

	if (m_states.back().second->IsTransparent() && m_states.size() > 1)
	{
		auto itr{ m_states.end() };
		--itr;
		while (itr != m_states.begin())
		{
			if (itr != m_states.end()) 
				if (!itr->second->IsTransparent()) 
					break;
				
			--itr;
		}
		for (; itr != m_states.end(); ++itr) 
		{
			m_shared->m_wind->GetRenderWindow()->setView(itr->second->GetView());
			itr->second->Draw();
		}
	}
	else 
		m_states.back().second->Draw();
}

bool StateManager::HasState(const StateType& type) 
{
	for (auto itr{ m_states.begin() }; itr != m_states.end(); ++itr)
	{
		if (itr->first == type)
		{
			auto removed{ std::find(m_toRemove.begin(), m_toRemove.end(), type) };
			if (removed == m_toRemove.end()) 
				return true; 

			return false;
		}
	}
	return false;
}

void StateManager::ProcessRequests() 
{
	while (m_toRemove.begin() != m_toRemove.end()) 
	{
		RemoveState(*m_toRemove.begin());
		m_toRemove.erase(m_toRemove.begin());
	}
}

void StateManager::SwitchTo(const StateType& type)
{
	m_shared->m_eventManager->SetCurrentState(type);
	for (auto itr{ m_states.begin() }; itr != m_states.end(); ++itr)
	{
		if (itr->first == type)
		{
			m_states.back().second->Deactivate();
			auto tmp_type{ itr->first };
			auto tmp_state{ std::move(itr->second) };
			m_states.erase(itr);
			tmp_state->Activate();
			m_shared->m_wind->GetRenderWindow()->setView(tmp_state->GetView());
			m_states.emplace_back(tmp_type, std::move(tmp_state));
			return;
		}
	}

	if (!m_states.empty())
		m_states.back().second->Deactivate(); 

	CreateState(type);
	m_states.back().second->Activate();
	m_shared->m_wind->GetRenderWindow()->setView(m_states.back().second->GetView());
}


void StateManager::CreateState(const StateType& type) 
{
	auto newState{ m_stateFactory.find(type) };
	if (newState == m_stateFactory.end()) 
		return; 

	auto state{ newState->second() };
	state->m_view = m_shared->m_wind->GetRenderWindow()->getDefaultView();
	state->OnCreate();
	m_states.emplace_back(type, std::move(state));
}

void StateManager::RemoveState(const StateType& type) 
{
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
	{
		if (itr->first == type) 
		{
			itr->second->OnDestroy();
			m_states.erase(itr);
			return;
		}
	}
}