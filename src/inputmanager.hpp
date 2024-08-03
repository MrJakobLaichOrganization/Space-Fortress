#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <cstdint>
#include <array>

class InputManager
{
  public:
	InputManager();
	void Update();
	void OnKeyPress(sf::Keyboard::Scancode key);
	void OnKeyRelease(sf::Keyboard::Scancode key);
	bool IsKeyDown(sf::Keyboard::Scancode key) const;
	bool IsKeyPressed(sf::Keyboard::Scancode key) const;

  private:
	std::array<std::uint8_t, sf::Keyboard::ScancodeCount> keysDown{};
	std::array<std::uint8_t, sf::Keyboard::ScancodeCount> keysLastDown{};
};