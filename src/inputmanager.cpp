#include "inputmanager.hpp"
#include <cstring>

InputManager::InputManager() {}

void InputManager::Update() { 
	keysLastDown = keysDown;
}
void InputManager::OnKeyPress(sf::Keyboard::Scancode key)
{
	keysDown[static_cast<std::uint32_t>(key)] = true;
}
void InputManager::OnKeyRelease(sf::Keyboard::Scancode key)
{ keysDown[static_cast<std::uint32_t>(key)] = false; }
bool InputManager::IsKeyDown(sf::Keyboard::Scancode key) const
{ return keysDown.at(static_cast<std::uint32_t>(key)); }
bool InputManager::IsKeyPressed(sf::Keyboard::Scancode key) const
{
	return keysDown.at(static_cast<std::uint32_t>(key)) && !keysLastDown.at(static_cast<std::uint32_t>(key));
}