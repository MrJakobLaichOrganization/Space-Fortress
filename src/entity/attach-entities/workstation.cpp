#include "workstation.hpp"

#include "entity/attach-entities/crewmate.hpp"
#include "entity/attach-entity.hpp"
#include "entity/root-entities/ship.hpp"

Workstation::Workstation(World* world,
                         Entity::Id id,
                         Location location,
                         Direction direction,
                         BlockGrid* grid,
                         BlockArchetypeIndex idx,
                         Direction workDir) :
    workStandDir(workDir),
    TileEntity(world, id, location, direction, grid, idx)
{
}
Workstation::~Workstation()
{
    assert(!entityUsing);
    assert(!task);
}

void Workstation::updateTask()
{
    const bool needTask = m_bills.size() > 0;
    if (task && !needTask)
    {
        dynamic_cast<Ship*>(parent)->removeTask(*task);
        task = nullptr;
    }
    else if (!task && needTask)
    {
        task = &dynamic_cast<Ship*>(parent)->addTask<WorkstationTask>(this);
    }
}

bool Workstation::doWork()
{
    if (m_bills.empty())
        return false;

    m_workCtr++;
    if (m_workCtr < m_workSpeed)
        return false;

    m_bills[0].workDone++;
    m_workCtr = 0;
    if (m_bills[0].workDone >= m_bills[0].workMax)
    {
        m_bills.erase(m_bills.begin());
        updateTask();
        return true;
    }
    return false;
}

WorkstationAct::WorkstationAct(Workstation* workstation) : workstation{workstation}
{
    auto ship = dynamic_cast<Ship*>(workstation->parent);
    const auto targetLocation = workstation->getLocation() + directionToLocation(workstation->workStandDir);
    const auto targetPosition = ship->locationToPosition(targetLocation) + Ship::blockSize / 2.f;
    moveAct = std::make_unique<MoveAct>(targetPosition);
}

Act::Status WorkstationAct::doAct(Crewmate& crewmate, sf::Time deltaTime)
{
    if (moveAct)
    {
        auto result = moveAct->doAct(crewmate, deltaTime);
        if (result != Status::Running)
        {
            moveAct = nullptr;
        }
        return result;
    }

    if (workstation->doWork())
    {
        return Status::Success;
    }

    return Status::Running;
}
