#include "tilemap.hpp"

#include <cassert>
#include <cmath>


TileRenderer::TileRenderer(const TileSheet& tileSheet, Bounds bounds, Size tileSize) :
    m_tileSheet{tileSheet},
    m_grid{bounds},
    m_tileSize{tileSize}
{
    m_verts.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_verts.resize(m_grid.getBounds().size.x * m_grid.getBounds().size.y * 6);
}

void TileRenderer::setTile(Location loc, Tile tile)
{
    auto& cell = m_grid.get(loc);
    if (cell == tile)
    {
        return;
    }

    cell = tile;

    const auto textureRect = m_tileSheet.getTileRect(tile.index);

    std::array uvPositions = {
        sf::Vector2f{textureRect.position.x, textureRect.position.y},
        sf::Vector2f{textureRect.position.x + textureRect.size.x, textureRect.position.y},
        sf::Vector2f{textureRect.position.x + textureRect.size.x, textureRect.position.y + textureRect.size.y},
        sf::Vector2f{textureRect.position.x, textureRect.position.y + textureRect.size.y},
    };

    sf::Vertex* vert = &m_verts[m_grid.locationToIndex(loc) * 6];

    vert[0].position = sf::Vector2f(loc.x * m_tileSize.x, loc.y * m_tileSize.y);
    vert[1].position = sf::Vector2f((loc.x + 1) * m_tileSize.x, loc.y * m_tileSize.y);
    vert[2].position = sf::Vector2f((loc.x + 1) * m_tileSize.x, (loc.y + 1) * m_tileSize.y);
    vert[3].position = sf::Vector2f(loc.x * m_tileSize.x, (loc.y + 1) * m_tileSize.y);

    // No point in rotating floor for now
    const auto rotationIndex = 4 - static_cast<int>(cell.direction);

    vert[0].texCoords = uvPositions[(rotationIndex + 0) % 4];
    vert[1].texCoords = uvPositions[(rotationIndex + 1) % 4];
    vert[2].texCoords = uvPositions[(rotationIndex + 2) % 4];
    vert[3].texCoords = uvPositions[(rotationIndex + 3) % 4];

    vert[4] = vert[0];
    vert[5] = vert[2];
}

void TileRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &m_tileSheet.getTexture();
    target.draw(m_verts, states);
}