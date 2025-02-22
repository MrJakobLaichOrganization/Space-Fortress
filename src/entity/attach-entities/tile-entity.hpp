#pragma once

#include "block.hpp"
#include "entity/attach-entity.hpp"

class TileEntity : public AttachEntity
{
public:
    TileEntity(class World* world, Id id, Location location, Direction direction, BlockGrid* grid, BlockArchetypeIndex idx = 0) :
        m_location{location},
        m_direction{direction},
        m_grid{grid},
        AttachEntity{world, id}
    {
        setArchetypeIdx(idx);
    }
    ~TileEntity() override;

    void setArchetypeIdx(BlockArchetypeIndex idx);

    [[nodiscard]] Location getLocation() const
    {
        return m_location;
    }

    [[nodiscard]] Direction getDirection() const
    {
        return m_direction;
    }

protected:
    Location m_location;
    Direction m_direction;
    BlockGrid* m_grid;
};