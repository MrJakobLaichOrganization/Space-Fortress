#include "inputmanager.hpp"
#include <cstring>

InputManager::InputManager() {}

void InputManager::Update() { 
	keysLastDown = keysDown;
}
void InputManager::OnKeyPress(sf::Keyboard::Scancode key)
{
	keysDown[key] = true;
}
void InputManager::OnKeyRelease(sf::Keyboard::Scancode key)
{
	keysDown[key] = false;
}
bool InputManager::IsKeyDown(sf::Keyboard::Scancode key) const
{
	return keysDown.at(key);
}
bool InputManager::IsKeyPressed(sf::Keyboard::Scancode key) const
{
	return keysDown.at(key) && !keysLastDown.at(key);
}