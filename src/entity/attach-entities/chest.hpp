#include "entity/attach-entities/machine.hpp"
#include "entity/attach-entity.hpp"
#include "item/inventory.hpp"

class Chest : public Machine
{
public:
    Chest(BlockGrid::Location location, std::size_t capacity, Direction direction = Direction::Up) :
        m_inv(capacity),
        Machine(location, direction)
    {
    }

    void update(sf::Time, class Ship&) override
    {
    }

    Inventory& getInventory()
    {
        return m_inv;
    }
    const Inventory& getInventory() const
    {
        return m_inv;
    }

private:
    Inventory m_inv;
};