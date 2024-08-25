#pragma once

#include "grid.hpp"
#include "units.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <atomic>
#include <string>

#include <cstdint>

class TileSheet
{
public:
    using TileIndex = std::uint32_t;

    TileSheet(const sf::Texture& texture, sf::Vector2u tileSize) : m_texture{texture}, m_tileSize{tileSize}
    {
    }

    [[nodiscard]] sf::FloatRect getTileRect(TileIndex tileIndex) const
    {
        const sf::Vector2f tileSize{m_tileSize};
        return {sf::Vector2f(tileIndex % (m_texture.getSize().x / m_tileSize.x) * m_tileSize.x,
                             tileIndex / (m_texture.getSize().x / m_tileSize.x) * m_tileSize.y),
                tileSize};
    }

    [[nodiscard]] const sf::Texture& getTexture() const
    {
        return m_texture;
    }

private:
    const sf::Texture& m_texture;
    sf::Vector2u m_tileSize;
};

class TileRenderer : public sf::Drawable
{
public:
    struct Tile
    {
        TileSheet::TileIndex index{};
        Direction direction{};

        bool operator==(const Tile&) const = default;
    };

    TileRenderer(const TileSheet& tileSheet, const sf::Vector2u& dimension, const sf::Vector2f& tileSize);

    void setTile(std::uint32_t idx, Tile tile);
    void setTile(const sf::Vector2u& pos, Tile tile);

private:
    const TileSheet& m_tileSheet;

    sf::Vector2f m_tileSize;
    Grid<Tile> m_grid;
    sf::VertexArray m_verts;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
