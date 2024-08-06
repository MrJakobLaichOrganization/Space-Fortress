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

	void setTile(std::uint32_t idx, std::uint32_t tileIdx);
	void setTile(const sf::Vector2u &pos, std::uint32_t tileIdx);

  private:
	const sf::Texture& m_tx;
	// Pixel sizes of individual tiles
	sf::Vector2<std::uint8_t> m_tileDims;
	// Tile count in width height
	sf::Vector2u m_tileCount;
	// Integer indices which map to a rectangle in atlas
	std::vector<std::uint32_t> m_tileIndices;

	sf::VertexArray m_verts;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
