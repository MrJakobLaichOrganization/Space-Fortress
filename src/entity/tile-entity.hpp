#pragma once

#include "block.hpp"
#include "entity/entity.hpp"

class TileEntity : public Entity
{
public:
    TileEntity(class World* world, Id id, BlockGrid::Location location, BlockGrid* grid, BlockArchetypeIndex idx = 0) :
        m_location{location},
        m_grid{grid},
        Entity{world, id}
    {
        setArchetypeIdx(idx);
    }
    virtual ~TileEntity();

    void setArchetypeIdx(BlockArchetypeIndex idx);
    
    [[nodiscard]] BlockGrid::Location getLocation() const
    {
        return m_location;
    }

protected:
    BlockGrid::Location m_location;
    BlockGrid* m_grid;
};