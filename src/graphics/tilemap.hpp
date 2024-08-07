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
	Tilemap(const sf::Texture& tx, const sf::Vector2<std::uint8_t> &tileDims, const sf::Vector2u &tileCount,
			const sf::Vector2u &renderDims = {64, 64});

	void setTile(std::uint32_t idx, std::uint32_t tileIdx);
	void setTile(const sf::Vector2u &pos, std::uint32_t tileIdx);

	[[nodiscard]] sf::Vector2u getTilesetDims() const;

  private:
	const sf::Texture& m_tx;
	const float m_scale = 1.0f;
	// Pixel sizes of individual tiles
	sf::Vector2<std::uint8_t> m_tileDims;
	// Tile size for rendering
	sf::Vector2u m_renderDims;
	// Integer indices which map to a rectangle in atlas
	std::vector<std::uint32_t> m_tileIndices;
	// How many tiles in tilemap
	sf::Vector2u m_tileCount;

	sf::VertexArray m_verts;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
