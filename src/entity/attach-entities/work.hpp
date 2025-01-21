#pragma once

#include "block.hpp"
#include "entity/entity.hpp"
#include "entity/attach-entities/machine.hpp"
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

    template<typename Archive>
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

class Workstation: public Machine {
public:
    bool inUse{false};
    Entity::Id entityUsing{0};
    std::vector<Bill> bills;
    Direction workStandDir;

    Workstation(BlockGrid::Location location, Direction direction, std::uint32_t tileIdx, Direction workDir = Direction::Down) :
        workStandDir(workDir), Machine(location, direction)
    {
        this->tileIdx = tileIdx;
    }

    void update(sf::Time deltaTime, class Ship& ship) override
    {
    }

    /// @brief Does work on the current bill
    /// @return finished the bill
    bool doWork()
    {
        if (bills.empty())
            return false;

        m_workCtr++;
        if (m_workCtr < m_workSpeed)
            return false;

        bills[0].workDone++;
        m_workCtr = 0;
        if (bills[0].workDone >= bills[0].workMax)
        {
            bills.erase(bills.begin());
            return true;
        }
        return false;
    }

    template <typename Archive>
    void add(Archive& ar)
    {
        ar(location, direction, tileIdx, inUse, entityUsing, bills);
    }

    private:
        std::uint16_t m_workSpeed = 5;    // How much ticks per work
        std::uint16_t m_workCtr = 0;
};