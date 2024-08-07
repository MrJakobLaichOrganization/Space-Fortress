#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <cstdint>
#include <array>

class InputManager
{
  public:
	void update();
	void onKeyPress(sf::Keyboard::Scancode key);
	void onKeyRelease(sf::Keyboard::Scancode key);
	[[nodiscard]] bool isKeyDown(sf::Keyboard::Scancode key) const;
	[[nodiscard]] bool isKeyPressed(sf::Keyboard::Scancode key) const;

  private:
	std::array<std::uint8_t, sf::Keyboard::ScancodeCount> m_keysDown{};
	std::array<std::uint8_t, sf::Keyboard::ScancodeCount> m_keysLastDown{};
};