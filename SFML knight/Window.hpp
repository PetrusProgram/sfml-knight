#pragma once
#include <string>
#include "EventManager.hpp"

class Window {
public:
	Window(){ Setup("Window", sf::Vector2u(640, 480)); }
	Window(const std::string& title, const sf::Vector2u& size) { Setup(title, size); }

	void BeginDraw() { m_window.clear(sf::Color::Black); }
	void EndDraw() { m_window.display(); }

	void Update();

	bool IsDone() { return m_isDone; }
	bool IsFullscreen() { return m_isFullscreen; }
	bool IsFocused() { return m_isFocused; }

	void ToggleFullscreen(EventDetails* details);
	void Close(EventDetails* details = nullptr) { m_isDone = true; }

	sf::RenderWindow* GetRenderWindow() { return &m_window; }
	EventManager* GetEventManager() { return &m_eventManager; }
	sf::Vector2u GetWindowSize() { return m_windowSize; }
	sf::FloatRect GetViewSpace();
private:
	void Setup(const std::string& title, const sf::Vector2u& size);
	void Create();

	sf::RenderWindow m_window;
	EventManager m_eventManager;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
	bool m_isDone{ false };
	bool m_isFullscreen{ false };
	bool m_isFocused{ true };
};