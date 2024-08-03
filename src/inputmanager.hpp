#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <cstdint>
#include <vector>

class InputManager
{
  public:
	InputManager();
	void Update();
	void PressKey(sf::Keyboard::Key key);
	void ReleaseKey(sf::Keyboard::Key key);
	bool IsKeyDown(sf::Keyboard::Key key) const;
	bool IsKeyPressed(sf::Keyboard::Key key) const;

  private:
	std::vector<std::uint8_t> keysDown;
	std::vector<std::uint8_t> keysLastDown;
};