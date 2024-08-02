#include "tilemap.hpp"
#include <cassert>
#include <cmath>

Tilemap::Tilemap(const sf::Texture &tx,
				 const sf::Vector2<std::uint8_t> &tileDims_,
				 const sf::Vector2u &tileCount_)
	: tx{tx}, tileDims{tileDims_}, tileCount{tileCount_}
{
	const std::uint32_t tileNumber = tileCount.x * tileCount.y;
	verts.setPrimitiveType(sf::PrimitiveType::Triangles);
	verts.resize(4 * tileCount.x * tileCount.y);
	tileIndices.resize(tileNumber, 0);

	for (std::size_t y = 0; y < tileCount.y; ++y)
	{
		for (std::size_t x = 0; x < tileCount.x; ++x)
		{
			sf::Vertex *quad = &verts[(y * tileCount.x + x) * 4];

			quad[0].position = sf::Vector2f(x * tileDims.x, y * tileDims.y);
			quad[1].position =
				sf::Vector2f((x + 1) * tileDims.x, y * tileDims.y);
			quad[2].position =
				sf::Vector2f((x + 1) * tileDims.x, (y + 1) * tileDims.y);
			quad[3].position =
				sf::Vector2f(x * tileDims.x, (y + 1) * tileDims.y);

			quad[0].texCoords = sf::Vector2f(0, 0);
			quad[1].texCoords = sf::Vector2f(tileDims.x, 0);
			quad[2].texCoords = sf::Vector2f(tileDims.x, tileDims.y);
			quad[3].texCoords = sf::Vector2f(0, tileDims.y);
		}
	}
}

void Tilemap::SetTile(std::uint32_t idx, std::uint32_t tileIdx) {
	assert(idx < tileIndices.size());
	if (tileIndices[idx] == tileIdx)
	{
		return;
	}

	tileIndices[idx] = tileIdx;

	auto tu = tileIdx % (tx.getSize().x / tileDims.x);
	auto tv = tileIdx / (tx.getSize().x / tileDims.x);
	sf::Vector2 pos{
		idx % tileCount.x,
		idx / tileCount.x
	};

	sf::Vertex *quad = &verts[(pos.y * tileCount.x + pos.x) * 4];

	quad[0].position = sf::Vector2f(pos.x * tileDims.x, pos.y * tileDims.y);
	quad[1].position =
		sf::Vector2f((pos.x + 1) * tileDims.x, pos.y * tileDims.y);
	quad[2].position =
		sf::Vector2f((pos.x + 1) * tileDims.x, (pos.y + 1) * tileDims.y);
	quad[3].position =
		sf::Vector2f(pos.x * tileDims.x, (pos.y + 1) * tileDims.y);

	quad[0].texCoords = sf::Vector2f(tu * tileDims.x, tv * tileDims.y);
	quad[1].texCoords = sf::Vector2f((tu + 1) * tileDims.x, tv * tileDims.y);
	quad[2].texCoords =
		sf::Vector2f((tu + 1) * tileDims.x, (tv + 1) * tileDims.y);
	quad[3].texCoords = sf::Vector2f(tu * tileDims.x, (tv + 1) * tileDims.y);
}
void Tilemap::SetTile(const sf::Vector2u &pos, std::uint32_t tileIdx)
{
	SetTile(pos.y * tileCount.x + pos.x, tileIdx);
}

void Tilemap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.texture = &tx;
	target.draw(verts, states);
}