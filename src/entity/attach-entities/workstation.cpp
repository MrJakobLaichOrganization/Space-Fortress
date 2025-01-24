#include "entity/attach-entities/crewmate.hpp"
#include "entity/attach-entity.hpp"
#include "work.hpp"

Workstation::Workstation(World* world,
                         Entity::Id id,
                         BlockGrid::Location location,
                         BlockGrid* grid,
                         BlockArchetypeIndex idx,
                         Direction /*direction*/
                         ,
                         Direction workDir) :
    parentWorld(world),
    workStandDir(workDir),
    TileEntity(world, id, location, grid, idx)
{
}
Workstation::~Workstation() = default;

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