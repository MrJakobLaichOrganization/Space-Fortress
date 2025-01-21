#include "tilemap.hpp"

#include <cassert>
#include <cmath>


TileRenderer::TileRenderer(const TileSheet& tileSheet, const sf::Vector2u& dimension, const sf::Vector2f& tileSize) :
    m_tileSheet{tileSheet},
    m_grid{dimension},
    m_tileSize{tileSize}
{
    m_verts.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_verts.resize(m_grid.getCount() * 6);

    m_vertsFloor.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertsFloor.resize(m_grid.getCount() * 6);
}

void TileRenderer::setTile(std::uint32_t idx, Tile tile, bool floor)
{
    auto& cell = m_grid.get(idx);
    if (cell == tile)
    {
        return;
    }

    if (floor)
    {
        cell.floorIndex = tile.floorIndex;
    }
    else
    {
        cell.index = tile.index;
        cell.direction = tile.direction;
    }

    const auto textureRect = m_tileSheet.getTileRect(tile.index);

    std::array uvPositions = {
        sf::Vector2f{textureRect.position.x, textureRect.position.y},
        sf::Vector2f{textureRect.position.x + textureRect.size.x, textureRect.position.y},
        sf::Vector2f{textureRect.position.x + textureRect.size.x, textureRect.position.y + textureRect.size.y},
        sf::Vector2f{textureRect.position.x, textureRect.position.y + textureRect.size.y},
    };

    const sf::Vector2 pos{idx % m_grid.getDimension().x, idx / m_grid.getDimension().x};

    sf::Vertex* vert = nullptr;
    if (floor)
    {
        vert = &m_vertsFloor[(pos.y * m_grid.getDimension().x + pos.x) * 6];
    }    
    else
    {
       vert = &m_verts[(pos.y * m_grid.getDimension().x + pos.x) * 6];
    }
    vert[0].position = sf::Vector2f(pos.x * m_tileSize.x, pos.y * m_tileSize.y);
    vert[1].position = sf::Vector2f((pos.x + 1) * m_tileSize.x, pos.y * m_tileSize.y);
    vert[2].position = sf::Vector2f((pos.x + 1) * m_tileSize.x, (pos.y + 1) * m_tileSize.y);
    vert[3].position = sf::Vector2f(pos.x * m_tileSize.x, (pos.y + 1) * m_tileSize.y);

    // No point in rotating floor for now
    const auto rotationIndex = 4 - static_cast<int>(floor ? Direction::Up : cell.direction);

    vert[0].texCoords = uvPositions[(rotationIndex + 0) % 4];
    vert[1].texCoords = uvPositions[(rotationIndex + 1) % 4];
    vert[2].texCoords = uvPositions[(rotationIndex + 2) % 4];
    vert[3].texCoords = uvPositions[(rotationIndex + 3) % 4];

    vert[4] = vert[0];
    vert[5] = vert[2];
}
void TileRenderer::setTile(const sf::Vector2u& pos, Tile tile, bool floor)
{
    setTile(m_grid.locationToIndex(pos), tile, floor);
}

void TileRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &m_tileSheet.getTexture();
    target.draw(m_vertsFloor, states);
    target.draw(m_verts, states);
}