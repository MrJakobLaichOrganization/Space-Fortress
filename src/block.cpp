#include "block.hpp"

std::vector<BlockArchetype> BlockGrid::m_blockArchetypes{};

BlockGrid::BlockGrid(sf::Vector2u dimensions, Tilemap* tilemap) : dims{dimensions}, m_tilemap{tilemap}
{
    m_blockData.resize(dims.x * dims.y);
    for (std::size_t i = 0; i < m_blockData.size(); ++i)
    {
        m_blockData[i].blockAchetypeIdx = 0;
        m_tilemap->setTile(i, m_blockArchetypes[0].tilemapIdx);
    }
}

void BlockGrid::setBlockType(BlockArchetypeIndex blockType, Index idx)
{
    m_blockData[idx].blockAchetypeIdx = blockType;

    if (m_tilemap)
    {
        m_tilemap->setTile(idx, getBlockArchetype(idx).tilemapIdx);
    }
}
void BlockGrid::setBlockType(BlockArchetypeIndex blockType, Location pos)
{
    setBlockType(blockType, pos.y * dims.x + pos.x);
}
void BlockGrid::setBlockType(std::string_view archetypeName, Location pos)
{
    setBlockType(getBlockArchetypeIdx(archetypeName), pos);
}

const BlockArchetype& BlockGrid::getBlockArchetype(Index idx) const
{
    return m_blockArchetypes[m_blockData[idx].blockAchetypeIdx];
}
const BlockArchetype& BlockGrid::getBlockArchetype(Location pos) const
{
    return getBlockArchetype(calculateIndex(pos));
}
BlockGrid::Index BlockGrid::getBlockArchetypeIdx(std::string_view name) const
{
    for (std::size_t i = 0; i < m_blockArchetypes.size(); ++i)
    {
        if (m_blockArchetypes[i].name == name)
        {
            return i;
        }
    }

    return static_cast<Index>(-1);
}

const BlockData& BlockGrid::getBlockData(Index idx) const
{
    return m_blockData[idx];
}
const BlockData& BlockGrid::getBlockData(Location pos) const
{
    return m_blockData[calculateIndex(pos)];
}
BlockData& BlockGrid::getBlockData(Index idx)
{
    return m_blockData[idx];
}
BlockData& BlockGrid::getBlockData(Location pos)
{
    return m_blockData[calculateIndex(pos)];
}

BlockGrid::Index BlockGrid::calculateIndex(Location pos) const
{
    return pos.y * dims.x + pos.x;
}