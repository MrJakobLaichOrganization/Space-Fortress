#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <cstdint>
#include <array>

class InputManager
{
  public:
	InputManager();
	void Update();
	void PressKey(sf::Keyboard::Scancode key);
	void ReleaseKey(sf::Keyboard::Scancode key);
	bool IsKeyDown(sf::Keyboard::Scancode key) const;
	bool IsKeyPressed(sf::Keyboard::Scancode key) const;

  private:
	std::array<std::uint8_t, sf::Keyboard::Scancode::ScancodeCount> keysDown{};
	std::array<std::uint8_t, sf::Keyboard::Scancode::ScancodeCount> keysLastDown{};
};