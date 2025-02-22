#include "crewmate.hpp"

#include "entity/root-entities/ship.hpp"
#include "pathfinding.hpp"
#include "world.hpp"

#include <iostream>

Crewmate::Crewmate(World* world, Id id, std::string_view name, Gender gender) :
    AttachEntity{world, id},
    m_name(name),
    m_gender(gender)
{
    m_birthTimestamp = world->getTime();
    localBounds = {sf::Vector2f{-16.f, -16.f}, sf::Vector2f{32.f, 32.f}};
}
Crewmate::~Crewmate()
{
    clearWorkstation();
}

void Crewmate::update(sf::Time deltaTime) // NOLINT
{
    work(deltaTime);
    step(deltaTime);
}

void Crewmate::step(sf::Time deltaTime)
{
    constexpr float stepEpsilon = 0.1f;

    const auto ship = dynamic_cast<Ship*>(parent);

    const auto targetPosition = ship->locationToPosition(targetLocation) + Ship::blockSize / 2.f;

    if ((targetPosition - getPosition()).length() <= stepEpsilon)
    {
        return;
    }

    if (m_steps.empty())
    {
        updatePathfinding();
    }

    if (m_steps.empty())
    {
        return;
    }

    auto currentStepPosition = ship->locationToPosition(m_steps[0]) + Ship::blockSize / 2.f;
    if ((currentStepPosition - getPosition()).length() <= stepEpsilon)
    {
        m_steps.erase(m_steps.begin());
        if (m_steps.empty())
        {
            return;
        }

        currentStepPosition = ship->locationToPosition(m_steps[0]) + Ship::blockSize / 2.f;
    }

    const auto dir = currentStepPosition - getPosition();
    const auto distance = dir.length();
    move(dir.normalized() * std::min(speed * deltaTime.asSeconds(), distance));
}

void Crewmate::work(sf::Time /*deltaTime*/)
{
    const Location gridLocation = posToGridLocation(getPosition(), Ship::blockSize);
    Ship* parentShip = dynamic_cast<Ship*>(parent);

    if (targetLocation == gridLocation)
    {
        switch (m_currentTask.type)
        {
            case TaskType::Idle:
                m_currentTask.type = TaskType::None;
                break;
            case TaskType::Work:
            {
                // If workstation invalid for some reason
                if (m_currentWorkstation == Entity::invalidID)
                {
                    m_currentTask.type = TaskType::None;
                    break;
                }
                if (auto* workstation = world->findEntity<Workstation>(m_currentWorkstation))
                {
                    if (workstation->doWork())
                    {
                        m_currentTask.type = TaskType::None;
                        workstation->inUse = false;
                        for (std::size_t i = 0; i < parentShip->takenTasks.size(); ++i)
                        {
                            if (parentShip->takenTasks[i] != m_currentTask)
                            {
                                continue;
                            }

                            parentShip->takenTasks.erase(parentShip->takenTasks.begin() + i);
                            break;
                        }
                    }
                }

                break;
            }
            default:
                break;
        }
    }

    if ((m_currentTask.type == TaskType::None || m_currentTask.type == TaskType::Idle) && !parentShip->tasks.empty())
    {
        m_currentTask = parentShip->tasks[0];

        parentShip->tasks.erase(parentShip->tasks.begin());
        parentShip->takenTasks.push_back(m_currentTask);

        targetLocation = m_currentTask.position;
    }
    else if (m_currentTask.type == TaskType::None || m_currentTask.type == TaskType::Idle)
    {
        for (auto* workstation : parentShip->getWorkstations())
        {
            if (workstation->inUse || workstation->bills.empty())
            {
                continue;
            }

            workstation->entityUsing = id;
            workstation->inUse = true;
            m_currentWorkstation = workstation->id;
            m_currentTask.type = TaskType::Work;
            m_currentTask.position = workstation->getLocation() + directionToLocation(workstation->workStandDir);
        }
        if (m_currentTask.type == TaskType::None)
        {
            m_currentTask.type = TaskType::Idle;
            m_currentTask.position = {gridLocation.x + 1, gridLocation.y + 1};
        }

        targetLocation = m_currentTask.position;
    }
}

void Crewmate::updatePathfinding()
{
    const Location gridLocation = posToGridLocation(getPosition(), Ship::blockSize);
    m_steps = dynamic_cast<Ship*>(parent)->pathfind(gridLocation, targetLocation);
}
void Crewmate::clearWorkstation()
{
    if (m_currentWorkstation == Entity::invalidID)
    {
        return;
    }

    if (m_currentTask.type == TaskType::Work)
    {
        if (auto* workstation = world->findEntity<Workstation>(m_currentWorkstation))
        {
            workstation->inUse = false;
        }

        m_currentWorkstation = Entity::invalidID;
        m_currentTask.type = TaskType::None;
        m_steps.clear();
    }
}

Time Crewmate::getAge() const
{
    return world->getTime() - m_birthTimestamp;
}

bool Crewmate::isAdult() const
{
    return getAge().year() >= adultYear;
}
