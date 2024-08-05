#pragma once

#include "graphics/tilemap.hpp"
#include "entity.hpp"
#include "crewmate.hpp"
#include "resources.hpp"
#include "block.hpp"

class Ship : public Entity
{
  public:
	Tilemap tilemap{Resources::Get().tilesTexture, {64, 64}, {32, 32}};
	BlockGrid grid{{32, 32}, &tilemap};
	std::vector<Crewmate> crewmates;

	Ship()
	{
		const auto floorTile = 10 * 30 + 10;

		grid.SetBlockType(6 * 30 + 10, {0, 0});
		grid.SetBlockType(6 * 30 + 12, {6, 0});

		grid.SetBlockType(8 * 30 + 10, {0, 7});
		grid.SetBlockType(8 * 30 + 12, {6, 7});

		for (std::uint32_t x = 0; x < 5; x++)
		{
			grid.SetBlockType(8 * 30 + 17, {x + 1, 0});
			grid.SetBlockType(9 * 30 + 10, {x + 1, 1});
			grid.SetBlockType(8 * 30 + 17, {x + 1, 7});
			grid.SetBlockType(7 * 30 + 14, {0, x + 1});
			grid.SetBlockType(7 * 30 + 14, {6, x + 1});
			for (std::uint32_t y = 0; y < 5; y++)
			{
				grid.SetBlockType(floorTile, {x + 1, y + 2});
			}
		}

		grid.SetBlockType(7 * 30 + 14, {0, 6});
		grid.SetBlockType(7 * 30 + 14, {6, 6});

		grid.SetBlockType(9 * 30 + 19, {2, 1});
	}

	void CreateCrewmate(Crewmate &&crewmate) { 
		crewmates.push_back(crewmate);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		states.transform *= getTransform();
		target.draw(tilemap, states);

		for (auto &crewmate : crewmates)
		{
			crewmate.draw(target, states);
		}
	}
	void Update(sf::Time delta) override
	{
		for (auto &crewmate : crewmates)
		{
			crewmate.Update(delta);
		}
	}
};