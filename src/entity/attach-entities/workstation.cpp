#include "workstation.hpp"

#include "entity/attach-entities/crewmate.hpp"
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
    if (m_task)
    {
        if (auto* ship = dynamic_cast<Ship*>(parent))
        {
            ship->removeTask(*m_task);
            m_task = nullptr;
        }
    }
    assert(!entityUsing);
}

void Workstation::updateTask()
{
    const bool needTask = !m_bills.empty();
    if (m_task && !needTask)
    {
        dynamic_cast<Ship*>(parent)->removeTask(*m_task);
        m_task = nullptr;
    }
    else if (!m_task && needTask)
    {
        m_task = &dynamic_cast<Ship*>(parent)->addTask<WorkstationTask>(this);
    }
}

bool Workstation::doWork()
{
    if (m_bills.empty())
        return false;

    auto& bill = m_bills[0];
    bill.workDone += m_workSpeed;
    if (bill.workDone >= bill.workMax)
    {
        if (auto worker = dynamic_cast<Crewmate*>(world->findEntity(entityUsing)))
        {
            worker->inventory.add(bill.itemToMake.type, bill.itemToMake.count);
        }

        m_bills.erase(m_bills.begin());
        updateTask();
        return true;
    }
    return false;
}

ActPtr WorkstationTask::start()
{
    const auto targetLocation = workstation->getLocation() + directionToLocation(workstation->workStandDir);
    const auto targetPosition = Ship::locationToPosition(targetLocation) + Ship::blockSize / 2.f;

    return std::make_unique<ActSequence>(std::make_unique<MoveAct>(targetPosition),
                                         std::make_unique<WorkstationAct>(workstation));
}

WorkstationAct::WorkstationAct(Workstation* workstation) : workstation{workstation}
{
}

Act::Status WorkstationAct::doAct(Crewmate& crewmate, sf::Time)
{
    workstation->entityUsing = crewmate.id;
    if (workstation->doWork())
    {
        return Status::Success;
    }

    return Status::Running;
}
