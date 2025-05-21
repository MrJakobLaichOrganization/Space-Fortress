#include "gui.hpp"

namespace
{
[[nodiscard]] sf::Vector2f newSpriteScale(const sf::Sprite& sprite, sf::Vector2u desiredSize)
{
    auto originalSize = sprite.getTexture().getSize();

    return sf::Vector2f{static_cast<float>(desiredSize.x) / originalSize.x,
                        static_cast<float>(desiredSize.y / originalSize.y)};
}
} // namespace

Image::Image(std::string_view imgPath, sf::Vector2f position, sf::Vector2f dims, GuiElement* parent) :
    m_tx{sf::Texture::loadFromFile(std::string{imgPath}).value()},
    m_sprite{m_tx},
    GuiElement(position, dims, parent)
{
    updateImageDimensions();
}

void Image::onResize(sf::Vector2u newDims)
{
    updateImageDimensions();
    GuiElement::onResize(newDims);
}

void Image::setPos(sf::Vector2f newPos)
{
    if (m_position == newPos)
    {
        return;
    }

    m_position = newPos;
    updateImageDimensions();
}
void Image::setSize(sf::Vector2f newSize)
{
    if (m_dims == newSize)
    {
        return;
    }

    m_dims = newSize;
    updateImageDimensions();
}
void Image::setImage(std::string_view imgPath)
{
}
void Image::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
    GuiElement::draw(target, states);
}
void Image::updateImageDimensions()
{
    const auto position = pixelPos();
    m_sprite.setPosition({position.x, position.y});
    m_sprite.setScale(newSpriteScale(m_sprite, static_cast<sf::Vector2u>(pixelSize())));
}