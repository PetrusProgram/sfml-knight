#include "EventManager.hpp"
#include <iostream>




bool EventManager::AddBinding(std::unique_ptr<Binding> binding)
{
	if (m_bindings.find(binding->m_name) != m_bindings.end())
		return false;

	return m_bindings.emplace(binding->m_name, std::move(binding)).second;
}

bool EventManager::RemoveBinding(std::string name) 
{
	auto itr{ m_bindings.find(name) };
	if (itr == m_bindings.end()) 
		return false; 

	m_bindings.erase(itr);
	return true;
}


void EventManager::HandleEvent(sf::Event& event)
{
	for (auto &b_itr : m_bindings)
	{
		auto bind{ b_itr.second.get() };
		for (auto &e_itr : bind->m_events) 
		{
			auto sfmlEvent{ static_cast<EventType>(event.type) };
			if (e_itr.first != sfmlEvent) 
				continue; 

			if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp) 
			{
				if (e_itr.second.m_code == event.key.code) 
				{
					if (bind->m_details.m_keyCode != -1) 
						bind->m_details.m_keyCode = e_itr.second.m_code;
					
					++(bind->m_count);
					break;
				}
			}
			else if (sfmlEvent == EventType::MButtonDown || sfmlEvent == EventType::MButtonUp) 
			{
				if (e_itr.second.m_code == event.mouseButton.button) 
				{
					bind->m_details.m_mouse.x = event.mouseButton.x;
					bind->m_details.m_mouse.y = event.mouseButton.y;
					if (bind->m_details.m_keyCode != -1) 
						bind->m_details.m_keyCode = e_itr.second.m_code;
					
					++(bind->m_count);
					break;
				}
			}
			else 
			{
				if (sfmlEvent == EventType::MouseWheel) 
					bind->m_details.m_mouseWheelDelta = event.mouseWheel.delta;
				
				else if (sfmlEvent == EventType::WindowResized) 
				{
					bind->m_details.m_size.x = event.size.width;
					bind->m_details.m_size.y = event.size.height;
				}
				else if (sfmlEvent == EventType::TextEntered) 
					bind->m_details.m_textEntered = event.text.unicode;
				
				++(bind->m_count);
			}
		}
	}
}

void EventManager::Update() 
{
	if (!m_hasFocus)  
		return; 

	for (auto &b_itr : m_bindings) 
	{
		auto bind{ b_itr.second.get() };
		for (auto &e_itr : bind->m_events)
		{
			switch (e_itr.first) 
			{
			case(EventType::Keyboard):
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e_itr.second.m_code))) 
				{
					if (bind->m_details.m_keyCode != -1) 
						bind->m_details.m_keyCode = e_itr.second.m_code;
					
					++(bind->m_count);
				}
				break;
			case(EventType::Mouse):
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button(e_itr.second.m_code)))
				{
					if (bind->m_details.m_keyCode != -1) 
						bind->m_details.m_keyCode = e_itr.second.m_code;
					
					++(bind->m_count);
				}
				break;
			default:
				break;
			}
		}

		if (bind->m_events.size() == bind->m_count)
		{
			auto stateCallbacks{ m_callbacks.find(m_currentState) };
			auto otherCallbacks{ m_callbacks.find(StateType(0)) };

			if (stateCallbacks != m_callbacks.end()) 
			{
				auto callItr{ stateCallbacks->second.find(bind->m_name) };
				if (callItr != stateCallbacks->second.end()) 
					callItr->second(&bind->m_details);	
			}

			if (otherCallbacks != m_callbacks.end()) {
				auto callItr{ otherCallbacks->second.find(bind->m_name) };
				if (callItr != otherCallbacks->second.end()) 
					callItr->second(&bind->m_details);
			}
		}
		bind->m_count = 0;
		bind->m_details.Clear();
	}
}

void EventManager::LoadBindings()
{
	static const std::string delimiter(":");
	std::ifstream bindings(Utils::GetWorkingDirectory() + "/Resources/keys.cfg.txt");
	if (!bindings) 
	{ 
		std::cout << "Failed loading keys.cfg." << std::endl;
		return;
	}
	std::string line;
	while (std::getline(bindings, line))
	{
		std::stringstream keystream(line);
		std::string callbackName;
		keystream >> callbackName;
		auto bind{ std::make_unique<Binding>(callbackName) };
		std::string keyval;
		while (keystream >> keyval)
		{
			int start{ 0 };
			auto end{ keyval.find(delimiter) };
			if (end == std::string::npos) 
				break; 

			auto type{ static_cast<EventType>(std::stoi(keyval.substr(start, end - start))) };
			int code{ std::stoi(keyval.substr(end + delimiter.length(), keyval.length())) };
			EventInfo eventInfo;
			eventInfo.m_code = code;
			bind->BindEvent(type, eventInfo);
		}

		AddBinding(std::move(bind));
	}
}
