#include "block.hpp"

std::vector<BlockArchetype> BlockGrid::m_blockArchetypes{};

BlockGrid::BlockGrid(sf::Vector2u dimensions, TileRenderer* tileRenderer) :
    Grid(dimensions),
    m_tileRenderer{tileRenderer}
{
    for (std::size_t i = 0; i < getCount(); ++i)
    {
        m_tileRenderer->setTile(i, {m_blockArchetypes[0].tilemapIdx});
    }
}

void BlockGrid::setBlockType(BlockArchetypeIndex blockType, Index idx)
{
    get(idx).blockAchetypeIdx = blockType;

    if (m_tileRenderer)
    {
        m_tileRenderer->setTile(idx, {getBlockArchetype(idx).tilemapIdx});
    }
}
void BlockGrid::setBlockType(BlockArchetypeIndex blockType, Location pos)
{
    setBlockType(blockType, locationToIndex(pos));
}
void BlockGrid::setBlockType(std::string_view archetypeName, Location pos)
{
    setBlockType(getBlockArchetypeIdx(archetypeName), pos);
}

const BlockArchetype& BlockGrid::getBlockArchetype(Index idx) const
{
    return m_blockArchetypes[get(idx).blockAchetypeIdx];
}
const BlockArchetype& BlockGrid::getBlockArchetype(Location pos) const
{
    return getBlockArchetype(locationToIndex(pos));
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
    return get(idx);
}
const BlockData& BlockGrid::getBlockData(Location pos) const
{
    return get(pos);
}
BlockData& BlockGrid::getBlockData(Index idx)
{
    return get(idx);
}
BlockData& BlockGrid::getBlockData(Location pos)
{
    return get(pos);
}