#include "block.hpp"

BlockGrid::BlockGrid(const sf::Vector2u &dimensions, Tilemap *tilemap_)
	: dims{dimensions}, tilemap{tilemap_}
{
	blockArchetypes.push_back(BlockArchetype{"Air", "", 1 * 30 + 1, false});
	blockArchetypes.push_back(BlockArchetype{"Wall", "", 9 * 30 + 10, true});
	blockArchetypes.push_back(BlockArchetype{"Wall_TL", "", 6 * 30 + 10, true});
	blockArchetypes.push_back(BlockArchetype{"Wall_TR", "", 6 * 30 + 12, true});
	blockArchetypes.push_back(BlockArchetype{"Wall_BL", "", 8 * 30 + 10, true});
	blockArchetypes.push_back(BlockArchetype{"Wall_BR", "", 8 * 30 + 12, true});
	blockArchetypes.push_back(BlockArchetype{"Wall_MUD", "", 7 * 30 + 14, true});
	blockArchetypes.push_back(BlockArchetype{"Wall_MLR", "", 8 * 30 + 17, true});
	blockArchetypes.push_back(BlockArchetype{"Floor", "", 10 * 30 + 10, false});
	blockArchetypes.push_back(BlockArchetype{"GateO2", "", 9 * 30 + 19, true});

	blockData.resize(dims.x * dims.y);
	for (std::size_t i = 0; i < blockData.size(); ++i)
	{
		blockData[i].blockAchetypeIdx = 0;
		tilemap->SetTile(i, blockArchetypes[0].tilemapIdx);
	}
}

void BlockGrid::SetBlockType(std::uint32_t blockType, std::uint32_t idx)
{
	blockData[idx].blockAchetypeIdx = blockType;

	if (tilemap)
	{
		tilemap->SetTile(idx, GetBlockArchetype(idx).tilemapIdx);
	}
}
void BlockGrid::SetBlockType(std::uint32_t blockType, const sf::Vector2u &pos)
{
	SetBlockType(blockType, pos.y * dims.x + pos.x);
}
void BlockGrid::SetBlockType(std::string_view archetype_name, const sf::Vector2u &pos)
{
	SetBlockType(GetBlockArchetypeIdx(archetype_name), pos);
}

const BlockArchetype &BlockGrid::GetBlockArchetype(std::uint32_t idx) const
{
	return blockArchetypes[blockData[idx].blockAchetypeIdx];
}
const BlockArchetype &BlockGrid::GetBlockArchetype(const sf::Vector2u &pos) const {
	return GetBlockArchetype(CalculateIndex(pos));
}
std::uint32_t BlockGrid::GetBlockArchetypeIdx(std::string_view name) const
{
	for (std::size_t i = 0; i < blockArchetypes.size(); ++i)
	{
		if (blockArchetypes[i].name == name)
		{
			return i;
		}
	}

	return static_cast<std::uint32_t>(-1);
}

const BlockData &BlockGrid::GetBlockData(std::uint32_t idx) const
{
	return blockData[idx];
}
const BlockData &BlockGrid::GetBlockData(const sf::Vector2u &pos) const
{
	return blockData[CalculateIndex(pos)];
}
BlockData &BlockGrid::GetBlockData(std::uint32_t idx) { return blockData[idx]; }
BlockData &BlockGrid::GetBlockData(const sf::Vector2u &pos)
{
	return blockData[CalculateIndex(pos)];
}

std::uint32_t BlockGrid::CalculateIndex(const sf::Vector2u &pos) const
{
	return pos.y * dims.x + pos.x;
}