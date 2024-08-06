#include "block.hpp"

BlockGrid::BlockGrid(const sf::Vector2u &dimensions, Tilemap *tilemap)
	: dims{dimensions}, m_tilemap{tilemap}
{
	m_blockArchetypes.push_back(BlockArchetype{"Air", "", BlockIdx({4, 1}), false});
	m_blockArchetypes.push_back(BlockArchetype{"Wall_TL", "", BlockIdx({3, 0}), true});
	m_blockArchetypes.push_back(BlockArchetype{"Wall_TR", "", BlockIdx({5, 0}), true});
	m_blockArchetypes.push_back(BlockArchetype{"Wall_BL", "", BlockIdx({3, 2}), true});
	m_blockArchetypes.push_back(BlockArchetype{"Wall_BR", "", BlockIdx({5, 2}), true});
	m_blockArchetypes.push_back(BlockArchetype{"Wall_MU", "", BlockIdx({4, 0}), true});
	m_blockArchetypes.push_back(BlockArchetype{"Wall_MD", "", BlockIdx({4, 2}), true});
	m_blockArchetypes.push_back(BlockArchetype{"Wall_MR", "", BlockIdx({5, 1}), true});
	m_blockArchetypes.push_back(BlockArchetype{"Wall_ML", "", BlockIdx({3, 1}), true});
	m_blockArchetypes.push_back(BlockArchetype{"Floor", "", BlockIdx({1, 1}), false});
	m_blockArchetypes.push_back(BlockArchetype{"DoorLocked", "", BlockIdx({7, 5}), true});
	m_blockArchetypes.push_back(BlockArchetype{"DoorClosed", "", BlockIdx({6, 2}), true});
	m_blockArchetypes.push_back(BlockArchetype{"DoorOpen", "", BlockIdx({7, 2}), true});

	m_blockData.resize(dims.x * dims.y);
	for (std::size_t i = 0; i < m_blockData.size(); ++i)
	{
		m_blockData[i].blockAchetypeIdx = 0;
		m_tilemap->setTile(i, m_blockArchetypes[0].tilemapIdx);
	}
}

void BlockGrid::setBlockType(std::uint32_t blockType, std::uint32_t idx)
{
	m_blockData[idx].blockAchetypeIdx = blockType;

	if (m_tilemap)
	{
		m_tilemap->setTile(idx, getBlockArchetype(idx).tilemapIdx);
	}
}
void BlockGrid::setBlockType(std::uint32_t blockType, const sf::Vector2u &pos)
{
	setBlockType(blockType, pos.y * dims.x + pos.x);
}
void BlockGrid::setBlockType(std::string_view archetypeName, const sf::Vector2u &pos)
{
	setBlockType(getBlockArchetypeIdx(archetypeName), pos);
}

const BlockArchetype &BlockGrid::getBlockArchetype(std::uint32_t idx) const
{
	return m_blockArchetypes[m_blockData[idx].blockAchetypeIdx];
}
const BlockArchetype &BlockGrid::getBlockArchetype(const sf::Vector2u &pos) const {
	return getBlockArchetype(calculateIndex(pos));
}
std::uint32_t BlockGrid::getBlockArchetypeIdx(std::string_view name) const
{
	for (std::size_t i = 0; i < m_blockArchetypes.size(); ++i)
	{
		if (m_blockArchetypes[i].name == name)
		{
			return i;
		}
	}

	return static_cast<std::uint32_t>(-1);
}

const BlockData &BlockGrid::getBlockData(std::uint32_t idx) const
{
	return m_blockData[idx];
}
const BlockData &BlockGrid::getBlockData(const sf::Vector2u &pos) const
{
	return m_blockData[calculateIndex(pos)];
}
BlockData &BlockGrid::getBlockData(std::uint32_t idx) { return m_blockData[idx]; }
BlockData &BlockGrid::getBlockData(const sf::Vector2u &pos)
{
	return m_blockData[calculateIndex(pos)];
}

std::uint32_t BlockGrid::calculateIndex(const sf::Vector2u &pos) const
{
	return pos.y * dims.x + pos.x;
}