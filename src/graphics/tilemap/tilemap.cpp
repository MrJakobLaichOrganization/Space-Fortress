#include "tilemap.h"
#include <cassert>

Tilemap::Tilemap(const std::string &fPath, std::uint8_t spriteW,
				 std::uint8_t spriteH, std::uint32_t windowW_,
				 std::uint32_t windowH_)
	: tileDims{spriteW, spriteH},
	  tileCount{windowW_ / spriteW, windowH_ / spriteH}
{
	if (windowW_ > windowH_)
	{
		factor.x = static_cast<float>(windowH_) / windowW_;
		factor.x = 1;
	}
	else if (windowH_ > windowW_)
	{
		factor.y = static_cast<float>(windowW_) / windowH_;
		factor.x = 1;
	}
	defaultFactor = factor;

	tx.loadFromFile(fPath);
	std::uint32_t tileNumber = tileCount.x * tileCount.y;
	verts.setPrimitiveType(sf::Quads);
	verts.resize(4 * tileCount.x * tileCount.y);
	tileIndices.resize(tileNumber, 0);

	for (std::size_t y = 0; y < tileCount.y; ++y)
	{
		for (std::size_t x = 0; x < tileCount.x; ++x)
		{
			sf::Vertex *quad = &verts[(y * tileCount.x + x) * 4];

			quad[0].position = sf::Vector2f(x * tileDims.x * factor.x,
											y * tileDims.y * factor.y);
			quad[1].position = sf::Vector2f((x + 1) * tileDims.x * factor.x,
											y * tileDims.y * factor.y);
			quad[2].position = sf::Vector2f((x + 1) * tileDims.x * factor.x,
											(y + 1) * tileDims.y * factor.y);
			quad[3].position = sf::Vector2f(x * tileDims.x * factor.x,
											(y + 1) * tileDims.y * factor.y);

			quad[0].texCoords = sf::Vector2f(0, 0);
			quad[1].texCoords = sf::Vector2f(tileDims.x, 0);
			quad[2].texCoords = sf::Vector2f(tileDims.x, tileDims.y);
			quad[3].texCoords = sf::Vector2f(0, tileDims.y);
		}
	}
}

void Tilemap::SetSprite(const sf::Vector2u &pos, std::uint32_t tileIdx)
{
	std::uint32_t idx = pos.y * tileCount.x + pos.x;
	assert(idx < tileIndices.size());
	if (tileIndices[idx] == tileIdx)
	{
		return;
	}

	tileIndices[idx] = tileIdx;

	auto tu = tileIdx % (tx.getSize().x / tileDims.x);
	auto tv = tileIdx / (tx.getSize().x / tileDims.x);

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
void Tilemap::Resize(std::uint32_t w, std::uint32_t h)
{
	//! TODO: you gotta account for the width and height of the tilemap too
	if (w > h)
	{
		factor.x = defaultFactor.x - (h / static_cast<float>(w));
		factor.y = defaultFactor.y;
	}
	else if (h > w)
	{
		factor.y = defaultFactor.x - w / static_cast<float>(h);
		factor.x = defaultFactor.y;
	}

	for (std::size_t y = 0; y < tileCount.y; ++y)
	{
		for (std::size_t x = 0; x < tileCount.x; ++x)
		{
			std::uint32_t tileIdx = tileIndices[y * tileCount.x + x];

			auto tu = tileIdx % (tx.getSize().x / tileDims.x);
			auto tv = tileIdx / (tx.getSize().x / tileDims.x);

			sf::Vertex *quad = &verts[(y * tileCount.x + x) * 4];

			quad[0].position = sf::Vector2f(x * tileDims.x * factor.x,
											y * tileDims.y * factor.y);
			quad[1].position = sf::Vector2f((x + 1) * tileDims.x * factor.x,
											y * tileDims.y * factor.y);
			quad[2].position = sf::Vector2f((x + 1) * tileDims.x * factor.x,
											(y + 1) * tileDims.y * factor.y);
			quad[3].position = sf::Vector2f(x * tileDims.x * factor.x,
											(y + 1) * tileDims.y * factor.y);

			quad[0].texCoords = sf::Vector2f(0, 0);
			quad[1].texCoords = sf::Vector2f(tileDims.x, 0);
			quad[2].texCoords = sf::Vector2f(tileDims.x, tileDims.y);
			quad[3].texCoords = sf::Vector2f(0, tileDims.y);
		}
	}
}

void Tilemap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.texture = &tx;
	target.draw(verts, states);
}