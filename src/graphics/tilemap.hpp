#pragma once

#include "grid.hpp"
#include "units.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <cstdint>

class TileSheet
{
public:
    struct TileDef
    {
        std::uint16_t index;
        bool isGeneric{};
        std::uint8_t atlasIndex{};

        bool operator==(const TileDef&) const = default;
    };

    TileSheet(const sf::Texture& texture, Dimension tileSize);

    [[nodiscard]] sf::FloatRect getTileRect(std::uint16_t index) const
    {
        const sf::Vector2f tileSize{m_tileSize};
        return {sf::Vector2f(index % m_originalDimension.x * m_tileSize.x, index / m_originalDimension.x * m_tileSize.y),
                tileSize};
    }

    [[nodiscard]] sf::FloatRect getGenericRect(std::uint16_t index) const
    {
        const sf::Vector2f tileSize{m_tileSize};
        return {m_genericOffset + sf::Vector2f(index % 16 * m_tileSize.x, index / 16 * m_tileSize.y), tileSize};
    }

    [[nodiscard]] const sf::Texture& getTexture() const
    {
        return m_texture;
    }

private:
    sf::Texture m_texture;
    sf::Vector2u m_tileSize;

    Dimension m_originalDimension;
    sf::Vector2f m_genericOffset;
};

class TileRenderer : public sf::Drawable
{
public:
    struct Tile
    {
        TileSheet::TileDef def{};
        Direction direction{};
        bool isDirty{};

        bool operator==(const Tile&) const = default;
    };

    TileRenderer(const TileSheet& tileSheet, Bounds bounds, Size tileSize);

    void setTile(Location loc, Tile tile);
    void updateRenderData();

private:
    const TileSheet& m_tileSheet;

    sf::Vector2f m_tileSize;
    OffsetGrid<Tile> m_grid;
    sf::VertexArray m_verts;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
