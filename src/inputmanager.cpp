#include "inputmanager.hpp"
#include <cstring>

void InputManager::update() { 
	m_keysLastDown = m_keysDown;
}
void InputManager::onKeyPress(sf::Keyboard::Scancode key)
{
	m_keysDown.at(static_cast<std::size_t>(key)) = true;
}
void InputManager::onKeyRelease(sf::Keyboard::Scancode key)
{
	m_keysDown.at(static_cast<std::size_t>(key)) = false;
}
bool InputManager::isKeyDown(sf::Keyboard::Scancode key) const
{
	return m_keysDown.at(static_cast<std::size_t>(key));
}
bool InputManager::isKeyPressed(sf::Keyboard::Scancode key) const
{
	return m_keysDown.at(static_cast<std::size_t>(key)) && !m_keysLastDown.at(static_cast<std::size_t>(key));
}