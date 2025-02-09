#include "block.hpp"

std::vector<BlockArchetype> BlockGrid::m_blockArchetypes{};

BlockGrid::BlockGrid(sf::Vector2u dimensions, TileRenderer* floorRenderer, TileRenderer* wallRenderer) :
    Grid(dimensions),
    m_floorRenderer{floorRenderer},
    m_mainRenderer{wallRenderer}
{

    const auto airIdx = m_blockArchetypes[0].tilemapIdx;
    for (std::size_t i = 0; i < getCount(); ++i)
    {
        m_floorRenderer->setTile(i, {airIdx});
        m_mainRenderer->setTile(i, {airIdx});
    }
}

void BlockGrid::setBlockType(BlockArchetypeIndex blockType, Index idx, Direction dir)
{
    get(idx).blockAchetypeIdx = blockType;

    if (m_mainRenderer)
    {
        m_mainRenderer->setTile(idx, {getBlockArchetype(idx).tilemapIdx, dir});
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

    if (m_mainRenderer)
    {
        m_mainRenderer->setTile(idx, {getBlockArchetype(idx).tilemapIdx, Direction::Up});
    }
}
void BlockGrid::clearBlockType(Location loc)
{
    clearBlockType(locationToIndex(loc));
}

void BlockGrid::setFloorType(BlockArchetypeIndex blockType, Index idx)
{
    get(idx).floorAchetypeIdx = blockType;

    if (m_floorRenderer)
    {
        m_floorRenderer->setTile(idx, {getFloorArchetype(idx).tilemapIdx});
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