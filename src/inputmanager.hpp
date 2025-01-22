#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <SFML/System.hpp>

#include <array>

#include <cstdint>

class InputManager
{
public:
    void update();
    void onKeyPress(sf::Keyboard::Scancode key);
    void onKeyRelease(sf::Keyboard::Scancode key);
    [[nodiscard]] bool isKeyDown(sf::Keyboard::Scancode key) const;
    [[nodiscard]] bool isKeyPressed(sf::Keyboard::Scancode key) const;

    sf::Vector2f screenMousePos;
    sf::Vector2f worldMousePos;

    bool leftMouseButonDown{};
    bool rightMouseButonDown{};

private:
    std::array<std::uint8_t, sf::Keyboard::ScancodeCount> m_keysDown{};
    std::array<std::uint8_t, sf::Keyboard::ScancodeCount> m_keysLastDown{};
};