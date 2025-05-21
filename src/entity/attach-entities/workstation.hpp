#pragma once

#include "block.hpp"
#include "entity/attach-entities/tile-entity.hpp"
#include "entity/entity.hpp"
#include "task/task.hpp"
#include "world.hpp"
#include "item/item.hpp"

#include <vector>

class Workstation;

/// @brief Job for a workstation,
struct Bill
{
    Item itemToMake;

    std::uint16_t workDone{0};
    std::uint16_t workMax;
    /// @brief Priority: 1 - lowest, 9 - highest
    std::uint8_t priority;

    Bill(std::uint16_t maxWork = std::numeric_limits<std::uint16_t>::max(), std::uint8_t prio = 5) :
        workMax(maxWork),
        priority(prio)
    {
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

class WorkstationAct : public Act
{
public:
    Workstation* workstation{};

    WorkstationAct(class Workstation* workstation);

    Status doAct(Crewmate& crewmate, sf::Time deltaTime) override;
};

class WorkstationTask : public Task
{
public:
    Workstation* workstation;

    WorkstationTask(Workstation* workstation) : workstation{workstation}
    {
        removeOnSuccess = false;
    }

    ActPtr start() override;
};

class Workstation : public TileEntity
{
public:
    Entity::Id entityUsing{0};
    Direction workStandDir;

    Workstation(World* world,
                Entity::Id id,
                Location location,
                Direction direction,
                BlockGrid* grid,
                BlockArchetypeIndex idx,
                Direction workDir = Direction::Up);
    ~Workstation() override;

    /// @brief Does work on the current bill
    /// @return finished the bill
    bool doWork();

    void updateTask();

    void addBill(Bill bill)
    {
        m_bills.push_back(bill);
        updateTask();
    }

    std::size_t getBillCount() const
    {
        return m_bills.size();
    }

    float getProgress() const
    {
        if (m_bills.empty())
        {
            return -1.f;
        }

        return static_cast<float>(m_bills[0].workDone) / m_bills[0].workMax;
    }

private:
    WorkstationTask* m_task{};
    std::vector<Bill> m_bills;
    std::uint16_t m_workSpeed = 1; // How much ticks per work
    std::uint16_t m_workCtr = 0;
};