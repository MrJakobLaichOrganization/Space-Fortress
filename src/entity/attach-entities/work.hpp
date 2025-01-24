#pragma once

#include "block.hpp"
#include "entity/entity.hpp"
#include "entity/tile-entity.hpp"
#include "world.hpp"

#include <vector>

/// @brief Job for a workstation,
struct Bill
{
    std::uint16_t workDone{0};
    std::uint16_t workMax;
    /// @brief Priority: 1 - lowest, 9 - highest
    std::uint8_t priority;

    Bill(std::uint16_t maxWork = std::numeric_limits<std::uint16_t>::max(), std::uint8_t prio = 5) :
        workMax(maxWork),
        priority(prio)
    {
    }

    template <typename Archive>
    void add(Archive& ar)
    {
        ar(workDone, workMax, priority);
    }
};
// For future work priority
struct BillCompare
{
    bool operator()(const Bill& a, const Bill& b) const
    {
        return a.priority > b.priority;
    }
};

class Workstation : public TileEntity
{
public:
    World* parentWorld{nullptr};
    bool inUse{false};
    Entity::Id entityUsing{0};
    std::vector<Bill> bills;
    Direction workStandDir;

    Workstation(World* world,
                Entity::Id id,
                BlockGrid::Location location,
                BlockGrid* grid,
                BlockArchetypeIndex idx,
                Direction dir = Direction::Up,
                Direction workDir = Direction::Down);
    ~Workstation() override;

    /// @brief Does work on the current bill
    /// @return finished the bill
    bool doWork();

    template <typename Archive>
    void add(Archive& ar)
    {
        ar(m_location, workStandDir, inUse, entityUsing, bills);
    }

private:
    std::uint16_t m_workSpeed = 5; // How much ticks per work
    std::uint16_t m_workCtr = 0;
};