#include "entity/tile-entity.hpp"

TileEntity::~TileEntity()
{
    m_grid->clearBlockType(m_location);
}

void TileEntity::setArchetypeIdx(BlockArchetypeIndex idx)
{
    m_grid->setBlockType(idx, m_location);
}