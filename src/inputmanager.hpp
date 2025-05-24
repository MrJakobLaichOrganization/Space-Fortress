#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <SFML/System.hpp>

#include <array>

class InputManager
{
public:
    void update();
    void onKeyPress(sf::Keyboard::Scancode key);
    void onKeyRelease(sf::Keyboard::Scancode key);

    void onButtonPress(sf::Mouse::Button button);
    void onButtonRelease(sf::Mouse::Button button);

    [[nodiscard]] bool isKeyDown(sf::Keyboard::Scancode key) const;    // Is key down
    [[nodiscard]] bool isKeyPressed(sf::Keyboard::Scancode key) const; // True on frame when clicked

    [[nodiscard]] bool isMouseDown(sf::Mouse::Button button) const;    // Is button down
    [[nodiscard]] bool isMousePressed(sf::Mouse::Button button) const; // True on frame when clicked

    sf::Vector2f screenMousePos;
    sf::Vector2f worldMousePos;

private:
    std::array<bool, sf::Keyboard::ScancodeCount> m_keysDown{};
    std::array<bool, sf::Keyboard::ScancodeCount> m_keysLastDown{};
    std::array<bool, sf::Mouse::ButtonCount> m_buttonsDown{};
    std::array<bool, sf::Mouse::ButtonCount> m_buttonsLastDown{};
};