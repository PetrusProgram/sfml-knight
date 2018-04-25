#include "Window.hpp"
#include <string>

void Window::Setup(const std::string& title, const sf::Vector2u& size)
{
	m_windowTitle = title;
	m_windowSize = size;

	m_eventManager.AddCallback(StateType(0), "Fullscreen_toggle", &Window::ToggleFullscreen, this);
	m_eventManager.AddCallback(StateType(0), "Window_close", &Window::Close, this);

	Create();
}

void Window::Create() 
{
	sf::Uint32 style{ sf::Style::Default };
	if (m_isFullscreen) 
		style = sf::Style::Fullscreen; 

	m_window.create(sf::VideoMode(m_windowSize.x, m_windowSize.y, 32), m_windowTitle, style);
}

sf::FloatRect Window::GetViewSpace()
{
	sf::Vector2f viewCenter(m_window.getView().getCenter());
	sf::Vector2f viewSize(m_window.getView().getSize());
	sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
	return viewSpace;
}

void Window::ToggleFullscreen(EventDetails* details) 
{
	m_isFullscreen = !m_isFullscreen;
	m_window.close();
	Create();
}
void Window::Update() 
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::LostFocus)
		{
			m_isFocused = false;
			m_eventManager.SetFocus(false);
		}
		else if (event.type == sf::Event::GainedFocus) 
		{
			m_isFocused = true;
			m_eventManager.SetFocus(true); 
		}
		m_eventManager.HandleEvent(event);
	}
	m_eventManager.Update();
}