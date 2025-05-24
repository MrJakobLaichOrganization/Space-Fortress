#include "inputmanager.hpp"

#include <utility>

void InputManager::update()
{
    m_keysLastDown = m_keysDown;
    m_buttonsLastDown = m_buttonsDown;
}
void InputManager::onKeyPress(sf::Keyboard::Scancode key)
{
    m_keysDown[std::to_underlying(key)] = true;
}
void InputManager::onKeyRelease(sf::Keyboard::Scancode key)
{
    m_keysDown[std::to_underlying(key)] = false;
}
bool InputManager::isKeyDown(sf::Keyboard::Scancode key) const
{
    return m_keysDown[std::to_underlying(key)];
}
bool InputManager::isKeyPressed(sf::Keyboard::Scancode key) const
{
    return m_keysDown[std::to_underlying(key)] && !m_keysLastDown[std::to_underlying(key)];
}

void InputManager::onButtonPress(sf::Mouse::Button button)
{
    m_buttonsDown[std::to_underlying(button)] = true;
}
void InputManager::onButtonRelease(sf::Mouse::Button button)
{
    m_buttonsDown[std::to_underlying(button)] = false;
}
bool InputManager::isMouseDown(sf::Mouse::Button button) const
{
    return m_buttonsDown[std::to_underlying(button)];
}
bool InputManager::isMousePressed(sf::Mouse::Button button) const
{
    return m_buttonsDown[std::to_underlying(button)] && !m_buttonsLastDown[std::to_underlying(button)];
}