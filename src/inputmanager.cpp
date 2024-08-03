#include "inputmanager.hpp"
#include <cstring>

InputManager::InputManager()
{
	keysDown.resize(sf::Keyboard::Key::KeyCount, 0);
	keysLastDown.resize(sf::Keyboard::Key::KeyCount, 0);
}

void InputManager::Update() { 
	keysLastDown = keysDown;
	std::memset(keysDown.data(), 0, keysDown.size());
}
void InputManager::PressKey(sf::Keyboard::Key key)
{
	keysDown[key] = true;
}
void InputManager::ReleaseKey(sf::Keyboard::Key key)
{
	keysDown[key] = false;
}
bool InputManager::IsKeyDown(sf::Keyboard::Key key) const
{
	return keysDown.at(key);
}
bool InputManager::IsKeyPressed(sf::Keyboard::Key key) const
{
	return keysDown.at(key) && !keysLastDown.at(key);
}