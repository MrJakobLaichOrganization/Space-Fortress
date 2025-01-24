#include "entity/attach-entities/crewmate.hpp"
#include "entity/attach-entity.hpp"
#include "work.hpp"

Workstation::Workstation(BlockGrid::Location location, World* world, Direction direction, Direction workDir) :
    parentWorld(world),
    workStandDir(workDir),
    Machine(location, direction)
{
}
Workstation::~Workstation()
{
}

bool Workstation::doWork()
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