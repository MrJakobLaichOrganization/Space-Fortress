#include "tilemap.hpp"

#include <SFML/Graphics.hpp>

#include <format>
#include <iostream>

#include <cassert>
#include <cmath>

sf::Texture generateGenericTiles(const TileSheet& tileSheet)
{
    const sf::Image sourceImg = tileSheet.getTexture().copyToImage();
    const sf::Vector2u tileSize(32, 32);
    const sf::Vector2u halfSize = tileSize / 2u;

    const auto cornerIndex = tileSize.componentWiseMul(sf::Vector2u(3, 0));
    const auto topIndex = tileSize.componentWiseMul(sf::Vector2u(3, 1));
    const auto sideIndex = tileSize.componentWiseMul(sf::Vector2u(4, 0));
    const auto insideIndex = tileSize.componentWiseMul(sf::Vector2u(8, 5));
    const auto insideCornerIndex = tileSize.componentWiseMul(sf::Vector2u(9, 6));

    std::vector<sf::Image> corners(0b1000, sf::Image(halfSize));
    for (int x = 0; x < corners.size(); x++)
    {
        const bool s = (x >> 0) & 1;
        const bool t = (x >> 2) & 1;

        const bool c = ((x >> 1) & 1) && s && t;

        if (!s && !t && !c)
        {
            std::ignore = corners[x].copy(sourceImg, {}, {sf::Vector2i(cornerIndex), sf::Vector2i(halfSize)});
        }
        else if (s && t && c)
        {
            std::ignore = corners[x].copy(sourceImg, {}, {sf::Vector2i(insideIndex), sf::Vector2i(halfSize)});
        }
        else if (s && t && !c)
        {
            std::ignore = corners[x].copy(sourceImg, {}, {sf::Vector2i(insideCornerIndex), sf::Vector2i(halfSize)});
        }
        else if (s && !t)
        {
            std::ignore = corners[x].copy(sourceImg, {}, {sf::Vector2i(sideIndex), sf::Vector2i(halfSize)});
        }
        else if (!s && t)
        {
            std::ignore = corners[x].copy(sourceImg, {}, {sf::Vector2i(topIndex), sf::Vector2i(halfSize)});
        }
    }

    sf::Image result(tileSize.componentWiseMul({16u, 16u}));
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            int i = y * 16 + x;
            i = i | (i << 8);

            for (int c = 0; c < 4; c++)
            {
                int cIndex = (i >> (c * 2)) & 0b111;
                const unsigned int row = (c & 2) > 0;
                const unsigned int col = ((c & 1) > 0) ^ row;
                if (row ^ col)
                {
                    cIndex = (cIndex & 0b010) | ((cIndex & 0b100) >> 2) | ((cIndex & 0b001) << 2);
                }
                auto cImage = corners[cIndex];
                if (col)
                {
                    cImage.flipHorizontally();
                }
                if (row)
                {
                    cImage.flipVertically();
                }

                std::ignore = result.copy(cImage,
                                          tileSize.componentWiseMul(sf::Vector2u(x, y)) +
                                              col * sf::Vector2u(halfSize.x, 0) + row * sf::Vector2u(0, halfSize.y));
            }
        }
    }

    return sf::Texture(result);
}

TileSheet::TileSheet(const sf::Texture& texture, Dimension tileSize) :
    m_texture(sf::Vector2u{1u, 1u}),
    m_tileSize{tileSize}
{
    m_texture = sf::Texture({texture.getSize() + sf::Vector2u(tileSize.x * 16u, tileSize.y * 16u)});
    m_texture.update(texture);
    auto extraTiles = generateGenericTiles(*this);
    m_texture.update(extraTiles, sf::Vector2u(0u, texture.getSize().y));

    m_texture.copyToImage().saveToFile("tilesheet.png");

    m_genericOffset = sf::Vector2f(0, texture.getSize().y);
    m_originalDimension = Dimension(texture.getSize()).componentWiseDiv(tileSize);
}

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
    cell.isDirty = true;

    if (cell.def.isGeneric)
    {
        for (const auto dir : Offset::dirs8)
        {
            if (m_grid.isValid(loc + dir))
            {
                auto& neigh = m_grid.get(loc + dir);
                if (neigh.def.isGeneric)
                {
                    neigh.isDirty = true;
                }
            }
        }
    }
}

void TileRenderer::updateRenderData()
{
    m_grid.forEach(
        [&](auto& tile, auto loc)
        {
            if (!tile.isDirty)
            {
                return;
            }

            tile.isDirty = false;

            auto textureRect = m_tileSheet.getTileRect(tile.def.index);

            if (tile.def.isGeneric)
            {
                std::uint16_t mapping{};
                std::uint8_t index{};

                for (const auto dir : Offset::dirs8)
                {
                    if (m_grid.isValid(loc + dir))
                    {
                        const auto& neigh = m_grid.get(loc + dir);
                        if (neigh.def.index == tile.def.index)
                        {
                            mapping |= 1 << index;
                        }
                    }

                    index++;
                }

                textureRect = m_tileSheet.getGenericRect(mapping);
            }

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

            const auto rotationIndex = 4 - static_cast<int>(tile.direction);

            vert[0].texCoords = uvPositions[(rotationIndex + 0) % 4];
            vert[1].texCoords = uvPositions[(rotationIndex + 1) % 4];
            vert[2].texCoords = uvPositions[(rotationIndex + 2) % 4];
            vert[3].texCoords = uvPositions[(rotationIndex + 3) % 4];

            vert[4] = vert[0];
            vert[5] = vert[2];
        });
}

void TileRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &m_tileSheet.getTexture();
    target.draw(m_verts, states);
}