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
	Tilemap(const std::string &fPath, std::uint8_t spriteW,
			std::uint8_t spriteH, std::uint32_t windowW, std::uint32_t windowH);

	const sf::Texture &GetTexture() const { return tx; }

	void SetSprite(const sf::Vector2u &pos, std::uint32_t idx);

	//! TODO: actually make it work, this shit sucks hard
	void Resize(std::uint32_t w, std::uint32_t h);

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
