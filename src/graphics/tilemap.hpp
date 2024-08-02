#pragma once

#include <atomic>
#include <cstdint>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <string>

class Tilemap : public sf::Drawable
{
  public:
	Tilemap(const sf::Texture& tx, const sf::Vector2<std::uint8_t> &tileDims,
			const sf::Vector2u &tileCount);

	void SetTile(std::uint32_t idx, std::uint32_t tileIdx);
	void SetTile(const sf::Vector2u &pos, std::uint32_t tileIdx);

  private:
	const sf::Texture& tx;
	// Pixel sizes of individual tiles
	sf::Vector2<std::uint8_t> tileDims;
	// Tile count in width height
	sf::Vector2u tileCount;
	// Integer indices which map to a rectangle in atlas
	std::vector<std::uint32_t> tileIndices;

	sf::VertexArray verts;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
