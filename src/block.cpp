#include "block.hpp"

std::vector<BlockArchetype> BlockGrid::m_blockArchetypes{};

BlockGrid::BlockGrid(Bounds bounds, TileRenderer* floorRenderer, TileRenderer* wallRenderer) :
    OffsetGrid(bounds),
    m_floorRenderer{floorRenderer},
    m_mainRenderer{wallRenderer}
{

    const auto airIdx = m_blockArchetypes[0].tilemapIdx;
    for (Index x = 0; x < getBounds().size.x; x++)
    {
        for (Index y = 0; y < getBounds().size.y; y++)
        {
            const auto loc = getBounds().position + Location{x, y};
            m_floorRenderer->setTile(loc, {airIdx});
            m_mainRenderer->setTile(loc, {airIdx});
        }
    }
}

void BlockGrid::setBlockType(BlockArchetypeIndex blockType, Location loc, Direction dir)
{
    get(loc).blockAchetypeIdx = blockType;

    if (m_mainRenderer)
    {
        m_mainRenderer->setTile(loc, {getBlockArchetype(loc).tilemapIdx, dir});
    }
}
void BlockGrid::setBlockType(std::string_view archetypeName, Location loc, Direction dir)
{
    setBlockType(getBlockArchetypeIdx(archetypeName), loc, dir);
}

void BlockGrid::clearBlockType(Location loc)
{
    get(loc).blockAchetypeIdx = BlockArchetype::airIndex;

    if (m_mainRenderer)
    {
        m_mainRenderer->setTile(loc, {getBlockArchetype(loc).tilemapIdx, Direction::Up});
    }
}

void BlockGrid::setFloorType(BlockArchetypeIndex blockType, Location loc)
{
    get(loc).floorAchetypeIdx = blockType;

    if (m_floorRenderer)
    {
        m_floorRenderer->setTile(loc, {getFloorArchetype(loc).tilemapIdx});
    }
}
void BlockGrid::setFloorType(std::string_view archetypeName, Location loc)
{
    setFloorType(getBlockArchetypeIdx(archetypeName), loc);
}

const BlockArchetype& BlockGrid::getBlockArchetype(Location loc) const
{
    return m_blockArchetypes[get(loc).blockAchetypeIdx];
}

BlockArchetypeIndex BlockGrid::getBlockArchetypeIdx(std::string_view name) const
{
    for (std::size_t i = 0; i < m_blockArchetypes.size(); ++i)
    {
        if (m_blockArchetypes[i].name == name)
        {
            return i;
        }
    }

    return static_cast<BlockArchetypeIndex>(-1);
}

const BlockArchetype& BlockGrid::getFloorArchetype(Location loc) const
{
    return m_blockArchetypes[get(loc).floorAchetypeIdx];
}

BlockArchetypeIndex BlockGrid::getFloorArchetypeIdx(std::string_view name) const
{
    for (std::size_t i = 0; i < m_blockArchetypes.size(); ++i)
    {
        if (m_blockArchetypes[i].name == name)
        {
            return i;
        }
    }

    return static_cast<BlockArchetypeIndex>(-1);
}

const BlockData& BlockGrid::getBlockData(Location loc) const
{
    return get(loc);
}

BlockData& BlockGrid::getBlockData(Location loc)
{
    return get(loc);
}