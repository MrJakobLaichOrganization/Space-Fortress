#include "tilemap.hpp"
#include <cassert>
#include <cmath>

Tilemap::Tilemap(const sf::Texture &tx,
				 const sf::Vector2<std::uint8_t> &tileDims, const sf::Vector2u &tileCount,
				 const sf::Vector2u &renderTileSize)
	: m_tx{tx}, m_tileDims{tileDims}, m_tileCount{tileCount}, m_renderDims{renderTileSize}
{
	const std::uint32_t tileNumber = m_tileCount.x * m_tileCount.y;
	m_verts.setPrimitiveType(sf::PrimitiveType::Triangles);
	m_verts.resize(m_tileCount.x * m_tileCount.y * 6);
	m_tileIndices.resize(tileNumber, 0);

	for (std::size_t y = 0; y < m_tileCount.y; ++y)
	{
		for (std::size_t x = 0; x < m_tileCount.x; ++x)
		{
			setTile(sf::Vector2u(x, y), 0);
		}
	}
}

void Tilemap::setTile(std::uint32_t idx, std::uint32_t tileIdx) {
	assert(idx < m_tileIndices.size());
	if (m_tileIndices[idx] == tileIdx)
	{
		return;
	}

	m_tileIndices[idx] = tileIdx;

	auto tu = tileIdx % (m_tx.getSize().x / m_tileDims.x);
	auto tv = tileIdx / (m_tx.getSize().x / m_tileDims.x);
	const sf::Vector2 pos{
		idx % m_tileCount.x,
		idx / m_tileCount.x
	};

	auto* vert = &m_verts[(pos.y * m_tileCount.x + pos.x) * 6];

	vert[0].position = sf::Vector2f(pos.x * m_renderDims.x, pos.y * m_renderDims.y);
	vert[1].position = sf::Vector2f((pos.x + 1) * m_renderDims.x, pos.y * m_renderDims.y);
	vert[2].position = sf::Vector2f((pos.x + 1) * m_renderDims.x, (pos.y + 1) * m_renderDims.y);
	vert[3].position = sf::Vector2f(pos.x * m_renderDims.x, (pos.y + 1) * m_renderDims.y);

	vert[0].texCoords = sf::Vector2f(tu * m_tileDims.x, tv * m_tileDims.y);
	vert[1].texCoords = sf::Vector2f((tu + 1) * m_tileDims.x, tv * m_tileDims.y);
	vert[2].texCoords = sf::Vector2f((tu + 1) * m_tileDims.x, (tv + 1) * m_tileDims.y);
	vert[3].texCoords = sf::Vector2f(tu * m_tileDims.x, (tv + 1) * m_tileDims.y);

	vert[4] = vert[0];
	vert[5] = vert[2];
}
void Tilemap::setTile(const sf::Vector2u &pos, std::uint32_t tileIdx)
{
	setTile(pos.y * m_tileCount.x + pos.x, tileIdx);
}

void Tilemap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.texture = &m_tx;
	target.draw(m_verts, states);
}
sf::Vector2u Tilemap::getTilesetDims() const { return sf::Vector2u(m_tx.getSize().x / m_tileDims.x, m_tx.getSize().y / m_tileDims.y); }