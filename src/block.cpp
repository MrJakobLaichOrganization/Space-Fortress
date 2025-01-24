#include "block.hpp"

std::vector<BlockArchetype> BlockGrid::m_blockArchetypes{};

BlockGrid::BlockGrid(sf::Vector2u dimensions, TileRenderer* tileRenderer) :
    Grid(dimensions),
    m_tileRenderer{tileRenderer}
{
    const auto airIdx = m_blockArchetypes[0].tilemapIdx;
    for (std::size_t i = 0; i < getCount(); ++i)
    {
        m_tileRenderer->setTile(i, {airIdx, airIdx}, false);
        m_tileRenderer->setTile(i, {airIdx, airIdx}, true);
    }
}

void BlockGrid::setBlockType(BlockArchetypeIndex blockType, Index idx, Direction dir)
{
    get(idx).blockAchetypeIdx = blockType;

    if (m_tileRenderer)
    {
        m_tileRenderer->setTile(idx, {0, getBlockArchetype(idx).tilemapIdx, dir}, false);
    }
}
void BlockGrid::setBlockType(BlockArchetypeIndex blockType, Location pos, Direction dir)
{
    setBlockType(blockType, locationToIndex(pos), dir);
}
void BlockGrid::setBlockType(std::string_view archetypeName, Location pos, Direction dir)
{
    setBlockType(getBlockArchetypeIdx(archetypeName), pos, dir);
}
void BlockGrid::clearBlockType(Index idx)
{
    get(idx).blockAchetypeIdx = BlockArchetype::airIndex;

    if (m_tileRenderer)
    {
        m_tileRenderer->setTile(idx, {0, getBlockArchetype(idx).tilemapIdx, Direction::Up}, false);
    }
}
void BlockGrid::clearBlockType(Location loc)
{
    clearBlockType(locationToIndex(loc));
}

void BlockGrid::setFloorType(BlockArchetypeIndex blockType, Index idx)
{
    get(idx).floorAchetypeIdx = blockType;

    if (m_tileRenderer)
    {
        m_tileRenderer->setTile(idx, {getFloorArchetype(idx).tilemapIdx}, true);
    }
}
void BlockGrid::setFloorType(BlockArchetypeIndex blockType, Location pos)
{
    setFloorType(blockType, locationToIndex(pos));
}
void BlockGrid::setFloorType(std::string_view archetypeName, Location pos)
{
    setFloorType(getBlockArchetypeIdx(archetypeName), pos);
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

const BlockArchetype& BlockGrid::getFloorArchetype(Index idx) const
{
    return m_blockArchetypes[get(idx).floorAchetypeIdx];
}
const BlockArchetype& BlockGrid::getFloorArchetype(Location pos) const
{
    return getFloorArchetype(locationToIndex(pos));
}
BlockGrid::Index BlockGrid::getFloorArchetypeIdx(std::string_view name) const
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