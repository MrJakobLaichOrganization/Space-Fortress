#include "gui.hpp"
#include "inputmanager.hpp"

#include <SFML/System/Vector2.hpp>

namespace
{
[[nodiscard]] sf::Vector2f newSpriteScale(const sf::Sprite& sprite, sf::Vector2u desiredSize)
{
    auto originalSize = sprite.getTexture().getSize();

    return sf::Vector2f{static_cast<float>(desiredSize.x) / originalSize.x,
                        static_cast<float>(desiredSize.y / originalSize.y)};
}
} // namespace

Button::Button(sf::Vector2f loc, sf::Vector2f dim, GuiElement* parent) :
    //m_text{font, sf::String(std::string{text})},
    GuiElement(loc, dim, GuiElement::FEAT_CLICKABLE, parent)
{
    /*auto textHalfDims = m_text.getLocalBounds().size;
    textHalfDims.x /= 2.f;
    
    m_text.setPosition({centerPos.x - textHalfDims.x, centerPos.y - textHalfDims.y});
    m_text.setFillColor(sf::Color{0, 0, 0});
    */
    const auto centerPos = calculateCenter();
    const auto topLeft = sf::Vector2f{centerPos.x - (dim.x / 2.f * getScreenDims().x),
                                      centerPos.y - (dim.y / 2.f * getScreenDims().y)};
    m_background.setPosition(topLeft);

    const auto newScale = newSpriteScale(m_background,
                                         sf::Vector2u(m_dims.x * getScreenDims().x, m_dims.y * getScreenDims().y));
    m_background.setScale(newScale);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_background, states);
    GuiElement::draw(target, states);
}
void Button::update(InputManager& input)
{
    GuiElement::update(input);
    // Normalize mouse pos
    auto mousePos = input.screenMousePos;
    mousePos.x /= getScreenDims().x;
    mousePos.y /= getScreenDims().y;

    // Return if mouse not in bounds
    if (mousePos.x < m_position.x || mousePos.x > (m_position.x + m_dims.x) || mousePos.y < m_position.y ||
        mousePos.y > (m_position.y + m_dims.y))
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
    /*
    if (!m_lastPressed && input.leftMouseButonDown)
    {
        if (m_onClick)
        {
            m_onClick();
        }
    }
*/
    m_lastPressed = input.leftMouseButonDown;
}

void Button::onResize(sf::Vector2u newDims)
{
    const auto centerPos = calculateCenter();

    m_background.setPosition(
        {centerPos.x - m_dims.x / 2.f * getScreenDims().x, centerPos.y - m_dims.y / 2.f * getScreenDims().y});
    m_background.setScale(
        newSpriteScale(m_background, sf::Vector2u(m_dims.x * getScreenDims().x, m_dims.y * getScreenDims().y)));

    GuiElement::onResize(newDims);
}
void Button::onClick(InputManager&)
{
    if(m_onClick){
        m_onClick();
    }
}
void Button::setPos(sf::Vector2f position)
{
    if (m_position == position)
    {
        return;
    }

    m_position = position;
    onResize(getScreenDims());
}