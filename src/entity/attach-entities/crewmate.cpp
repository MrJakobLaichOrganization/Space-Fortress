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
}
Crewmate::~Crewmate()
{
    if (m_currentWorkstation)
    {
        m_currentWorkstation->inUse = false;
    }
}

void Crewmate::update(sf::Time deltaTime) // NOLINT
{
    work(deltaTime);
    step(deltaTime);
}

void Crewmate::step(sf::Time deltaTime)
{
    const BlockGrid::Location gridLocation = posToGridLocation(getPosition(), static_cast<sf::Vector2u>(Ship::blockSize));

    if (targetLocation == gridLocation)
    {
        return;
    }

    if (m_steps.empty())
    {
        updatePathfinding();
    }

    if (m_steps.size() < 2)
    {
        return;
    }

    auto currentStep = m_steps[0];
    auto nextStep = m_steps[1];
    if (nextStep == gridLocation)
    {
        currentStep = m_steps[0];
        m_steps.erase(m_steps.begin());
        if (m_steps.size() > 1)
        {
            nextStep = m_steps[1];
        }
    }

    if (currentStep != gridLocation)
    {
        updatePathfinding();
        return;
    }

    const auto nextPosition = dynamic_cast<Ship*>(parent)->locationToPosition(nextStep) + Ship::blockSize / 2.f;
    move((nextPosition - getPosition()).normalized() * speed * deltaTime.asSeconds());
}

void Crewmate::work(sf::Time /*deltaTime*/)
{
    static const sf::Vector2i dirOffsets[] = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0},
    };
    const BlockGrid::Location gridLocation = posToGridLocation(getPosition(), static_cast<sf::Vector2u>(Ship::blockSize));
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
                if (m_currentWorkstation->doWork())
                {
                    m_currentTask.type = TaskType::None;
                    m_currentWorkstation->inUse = false;
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
                continue;

            workstation->entityUsing = id;
            workstation->inUse = true;
            this->m_currentWorkstation = workstation;
            this->m_currentTask.type = TaskType::Work;
            m_currentTask.position = static_cast<sf::Vector2u>(
                sf::Vector2i(workstation->getLocation()) + dirOffsets[static_cast<std::uint8_t>(workstation->workStandDir)]);
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
    const BlockGrid::Location gridLocation = posToGridLocation(getPosition(), static_cast<sf::Vector2u>(Ship::blockSize));
    m_steps = dynamic_cast<Ship*>(parent)->pathfind(gridLocation, targetLocation);
}
void Crewmate::clearWorkstation()
{
    if (!m_currentWorkstation)
    {
        return;
    }

    if (m_currentTask.type == TaskType::Work)
    {
        m_currentTask.type = TaskType::None;
        m_currentWorkstation = nullptr;
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
