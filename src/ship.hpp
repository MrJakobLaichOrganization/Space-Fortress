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
		const auto floorTile = grid.GetBlockArchetypeIdx("Floor");
		const auto floorMidUD = grid.GetBlockArchetypeIdx("Wall_MUD");

		grid.SetBlockType(grid.GetBlockArchetypeIdx("Wall_TL"), {0, 0});
		grid.SetBlockType(grid.GetBlockArchetypeIdx("Wall_MLR"), {8, 0});
		grid.SetBlockType(grid.GetBlockArchetypeIdx("Wall_TR"), {6, 0});

		grid.SetBlockType(grid.GetBlockArchetypeIdx("Wall_BL"), {0, 7});
		grid.SetBlockType(grid.GetBlockArchetypeIdx("Wall_BR"), {6, 7});

		for (std::uint32_t x = 0; x < 5; x++)
		{
			grid.SetBlockType(grid.GetBlockArchetypeIdx("Wall_MLR"), {x + 1, 0});
			grid.SetBlockType(grid.GetBlockArchetypeIdx("Wall"), {x + 1, 1});
			grid.SetBlockType(grid.GetBlockArchetypeIdx("Wall_MLR"), {x + 1, 7});
			grid.SetBlockType(floorMidUD, {0, x + 1});
			grid.SetBlockType(floorMidUD, {6, x + 1});
			for (std::uint32_t y = 0; y < 5; y++)
			{
				grid.SetBlockType(floorTile, {x + 1, y + 2});
			}
		}

		grid.SetBlockType(floorMidUD, {0, 6});
		grid.SetBlockType(floorMidUD, {6, 6});

		grid.SetBlockType(grid.GetBlockArchetypeIdx("GateO2"), {2, 1});
	}

	void CreateCrewmate(Crewmate &&crewmate) { 
		crewmates.emplace_back(std::move(crewmate));
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