#include "block.hpp"

BlockGrid::BlockGrid(const sf::Vector2u &dimensions, Tilemap *tilemap_)
	: dims{dimensions}, tilemap{tilemap_}
{
	blockArchetypes.push_back(BlockArchetype{"Dirt", "Dirt block", 4});
	blockArchetypes.push_back(BlockArchetype{"Stone", "Stone block", 6});
	blockArchetypes.push_back(BlockArchetype{"Stone wall", "Stone wall", 10});
	blockArchetypes.push_back(
		BlockArchetype{"Wooden floor", "Floor made out of wood", 43});

	blockData.resize(dims.x * dims.y);
	for (std::size_t i = 0; i < blockData.size(); ++i)
	{
		blockData[i].blockAchetypeIdx = 3;
	}
}

void BlockGrid::SetBlockType(std::uint32_t blockType, std::uint32_t idx)
{
	blockData[idx].blockAchetypeIdx = blockType;

	if (tilemap)
	{
		tilemap->SetTile(idx, blockType);
	}
}
void BlockGrid::SetBlockType(std::uint32_t blockType, const sf::Vector2u &pos)
{
	SetBlockType(blockType, pos.y * dims.x + pos.x);
}

const BlockArchetype &BlockGrid::GetBlockArchetype(std::uint32_t idx) const
{
	return blockArchetypes[blockData[idx].blockAchetypeIdx];
}
const BlockArchetype &BlockGrid::GetBlockArchetype(const sf::Vector2u &pos) const {
	return GetBlockArchetype(CalculateIndex(pos));
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