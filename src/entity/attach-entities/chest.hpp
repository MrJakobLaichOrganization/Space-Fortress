#include "entity/tile-entity.hpp"
#include "item/inventory.hpp"

class Chest : public TileEntity
{
public:
    Chest(class World* world,
          Id id,
          BlockGrid::Location location,
          BlockGrid* grid,
          BlockArchetypeIndex idx,
          Direction direction = Direction::Up,
          std::size_t capacity = 100) :
        m_inv(capacity),
        TileEntity(world, id, location, grid, idx)
    {
    }

    [[nodiscard]] Inventory& getInventory()
    {
        return m_inv;
    }
    [[nodiscard]] const Inventory& getInventory() const
    {
        return m_inv;
    }

private:
    Inventory m_inv;
};