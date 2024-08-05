#include "inputmanager.hpp"
#include <cstring>

InputManager::InputManager() {}

void InputManager::Update() { 
	keysLastDown = keysDown;
}
void InputManager::OnKeyPress(sf::Keyboard::Scancode key)
{
	keysDown.at(static_cast<std::size_t>(key)) = true;
}
void InputManager::OnKeyRelease(sf::Keyboard::Scancode key)
{
	keysDown.at(static_cast<std::size_t>(key)) = false;
}
bool InputManager::IsKeyDown(sf::Keyboard::Scancode key) const
{
	return keysDown.at(static_cast<std::size_t>(key));
}
bool InputManager::IsKeyPressed(sf::Keyboard::Scancode key) const
{
	return keysDown.at(static_cast<std::size_t>(key)) && !keysLastDown.at(static_cast<std::size_t>(key));
}