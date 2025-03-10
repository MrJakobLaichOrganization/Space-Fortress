#include "gui.hpp"

#include <iostream>

namespace
{
[[nodiscard]] sf::Vector2f newSpriteScale(const sf::Sprite& sprite, sf::Vector2u desiredSize)
{
    auto originalSize = sprite.getTexture().getSize();

    return sf::Vector2f{static_cast<float>(desiredSize.x) / originalSize.x,
                        static_cast<float>(desiredSize.y / originalSize.y)};
}
} // namespace

TextButton::TextButton(std::string_view text, const sf::Font& font, sf::Vector2f loc, sf::Vector2f dim, GuiElement* parent) :
    m_text{font, sf::String(std::string{text})},
    GuiElement(loc, dim, parent)
{
    auto textHalfDims = m_text.getLocalBounds().size;
    textHalfDims.x /= 2.f;
    const auto centerPos = calculateCenter();

    m_text.setPosition({centerPos.x - textHalfDims.x, centerPos.y - textHalfDims.y});
    m_text.setFillColor(sf::Color{0, 0, 0});

    m_background.setPosition({centerPos.x - dim.x / 2.f * getScreenDims().x, centerPos.y - dim.y / 2.f * getScreenDims().y});
    m_background.setScale(
        newSpriteScale(m_background, sf::Vector2u(m_dims.x * getScreenDims().x, m_dims.y * getScreenDims().y)));
}

void TextButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_background);
    target.draw(m_text);
}
void TextButton::update(InputManager& input)
{
    // Normalize mouse pos
    auto mousePos = input.screenMousePos;
    mousePos.x /= getScreenDims().x;
    mousePos.y /= getScreenDims().y;

    // Return if mouse not in bounds
    if (!(mousePos.x >= m_position.x && mousePos.x <= (m_position.x + m_dims.x)) ||
        !(mousePos.y >= m_position.y && mousePos.y <= (m_position.y + m_dims.y)))
    {
        m_hovering = m_lastPressed = false;
        return;
    }

    if (!m_hovering)
    {
        m_hovering = true;
        if (m_onHover)
        {
            m_onHover();
        }
    }
    // If just pressed
    if (!m_lastPressed && input.leftMouseButonDown)
    {
        if (m_onClick)
        {
            m_onClick();
        }
    }

    m_lastPressed = input.leftMouseButonDown;
}

void TextButton::onResize(sf::Vector2u newScreenSize)
{
    updateTextDimensions();
}
void TextButton::setPos(sf::Vector2f position)
{
    if (m_position == position)
    {
        return;
    }

    m_position = position;
    updateTextDimensions();
}

void TextButton::setText(std::string_view text)
{
    m_text.setString(std::string{text});
    updateTextDimensions();
}
void TextButton::updateTextDimensions()
{
    auto textHalfDims = m_text.getLocalBounds().size;
    textHalfDims.x /= 2.f;
    const auto centerPos = calculateCenter();

    m_background.setPosition(
        {centerPos.x - m_dims.x / 2.f * getScreenDims().x, centerPos.y - m_dims.y / 2.f * getScreenDims().y});
    m_text.setPosition({centerPos.x - textHalfDims.x, centerPos.y - textHalfDims.y});

    m_background.setScale(
        newSpriteScale(m_background, sf::Vector2u(m_dims.x * getScreenDims().x, m_dims.y * getScreenDims().y)));
}

ImageButton::ImageButton(std::string_view imgDir, sf::Vector2f loc, sf::Vector2f dim, GuiElement* parent) :
    m_spriteTx{sf::Texture::loadFromFile(imgDir).value()},
    m_img{m_spriteTx},
    GuiElement(loc, dim, parent)
{
    updateImageDimensions();
}

void ImageButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_background);
    target.draw(m_img);
}
void ImageButton::update(InputManager& input)
{
    // Normalize mouse pos
    auto mousePos = input.screenMousePos;
    mousePos.x /= getScreenDims().x;
    mousePos.y /= getScreenDims().y;

    // Return if mouse not in bounds
    if (!(mousePos.x >= m_position.x && mousePos.x <= (m_position.x + m_dims.x)) ||
        !(mousePos.y >= m_position.y && mousePos.y <= (m_position.y + m_dims.y)))
    {
        m_hovering = m_lastPressed = false;
        return;
    }

    if (!m_hovering)
    {
        m_hovering = true;
        if (m_onHover)
        {
            m_onHover();
        }
    }
    // If just pressed
    if (!m_lastPressed && input.leftMouseButonDown)
    {
        if (m_onClick)
        {
            m_onClick();
        }
    }

    m_lastPressed = input.leftMouseButonDown;
}

void ImageButton::onResize(sf::Vector2u newScreenSize)
{
    updateImageDimensions();
}
void ImageButton::setPos(sf::Vector2f position)
{
    if (m_position == position)
    {
        return;
    }

    m_position = position;
    updateImageDimensions();
}

void ImageButton::updateImageDimensions()
{
    const auto centerPos = calculateCenter();
    auto screenDims = getScreenDims();

    m_background.setPosition({centerPos.x - m_dims.x / 2.f * screenDims.x, centerPos.y - m_dims.y / 2.f * screenDims.y});
    m_img.setPosition({centerPos.x - m_dims.x / 2.f * screenDims.x, centerPos.y - m_dims.y / 2.f * screenDims.y});

    m_background.setScale(newSpriteScale(m_background, sf::Vector2u(m_dims.x * screenDims.x, m_dims.y * screenDims.y)));
    m_img.setScale(newSpriteScale(m_img, sf::Vector2u(m_dims.x * screenDims.x, m_dims.y * screenDims.y)));
}