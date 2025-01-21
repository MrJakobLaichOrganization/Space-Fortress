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

void Crewmate::update(sf::Time deltaTime) // NOLINT
{
    step(deltaTime);
}

void Crewmate::step(sf::Time deltaTime)
{
    static const sf::Vector2i dirOffsets[] = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0},
    };
    const BlockGrid::Location gridLocation = posToGridLocation(getPosition(), static_cast<sf::Vector2u>(Ship::blockSize));
    bool taskAdded = true;
    Ship* parentShip = dynamic_cast<Ship*>(parent);

    if ((m_currentTask.type == TaskType::None || m_currentTask.type == TaskType::Idle) && !parentShip->tasks.empty())
    {
        m_currentTask = parentShip->tasks[0];
        parentShip->tasks.erase(parentShip->tasks.begin());

        targetLocation = m_currentTask.position;
    }
    else if (m_currentTask.type == TaskType::None || m_currentTask.type == TaskType::Idle)
    {
        for (auto *workstation : parentShip->getWorkstations())
        {
            if (workstation->inUse || workstation->bills.empty())
                continue;

            workstation->entityUsing = id;
            workstation->inUse = true;
            this->m_currentWorkstation = workstation;
            this->m_currentTask.type = TaskType::Work;
            m_currentTask.position = static_cast<sf::Vector2u>(sf::Vector2i(workstation->location) + dirOffsets[static_cast<std::uint8_t>(workstation->workStandDir)]);
        }
        if (m_currentTask.type == TaskType::None)
        {
            m_currentTask.type = TaskType::Idle;
            m_currentTask.position = {gridLocation.x + 1, gridLocation.y + 1};
        }

        targetLocation = m_currentTask.position;
    }
    else
    {
        taskAdded = false;
    }

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
                }

                break;
            }
            default:
                break;
        }
        return;
    }

    if (m_steps.empty() || taskAdded)
    {
        updatePathfinding();

        while (m_steps.empty() && m_currentTask.type == TaskType::Idle && targetLocation.y < 31)
        {
            updatePathfinding();
            targetLocation.y++;
        }
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
        nextStep = m_steps[1];
    }

    if (currentStep != gridLocation)
    {
        updatePathfinding();
        return;
    }

    const auto nextPosition = dynamic_cast<Ship*>(parent)->locationToPosition(nextStep) + Ship::blockSize / 2.f;
    move((nextPosition - getPosition()).normalized() * speed * deltaTime.asSeconds());
}

void Crewmate::updatePathfinding()
{
    const BlockGrid::Location gridLocation = posToGridLocation(getPosition(), static_cast<sf::Vector2u>(Ship::blockSize));
    m_steps = dynamic_cast<Ship*>(parent)->pathfind(gridLocation, targetLocation);
}

Time Crewmate::getAge() const
{
    return world->getTime() - m_birthTimestamp;
}

bool Crewmate::isAdult() const
{
    return getAge().year() >= adultYear;
}
