#include "entity/attach-entities/crewmate.hpp"
#include "entity/attach-entity.hpp"
#include "work.hpp"

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
    if (!inUse)
    {
        return;
    }
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