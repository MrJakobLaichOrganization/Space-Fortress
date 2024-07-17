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
	Tilemap(const std::string &fPath, const sf::Vector2<std::uint8_t> &tileDims,
			const sf::Vector2u &windowDims);

	const sf::Texture &GetTexture() const { return tx; }

	void SetTile(const sf::Vector2u &pos, std::uint32_t idx);

	//! TODO: actually make it work, this shit sucks hard
	void Resize(const sf::Vector2u &dims);

  private:
	sf::Texture tx;
	// Pixel sizes of individual tiles
	sf::Vector2<std::uint8_t> tileDims;
	// Factor by which to multiply the sizes
	sf::Vector2f defaultFactor{1, 1};
	sf::Vector2f factor{1, 1};
	// Tile count in width height
	sf::Vector2u tileCount;
	// Integer indices which map to a rectangle in atlas
	std::vector<std::uint32_t> tileIndices;

	sf::VertexArray verts;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
