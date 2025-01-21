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

    if ((currentTask.type == TaskType::None || currentTask.type == TaskType::Idle) && parentShip->tasks.size())
    {
        currentTask = parentShip->tasks[0];
        parentShip->tasks.erase(parentShip->tasks.begin());

        targetLocation = currentTask.position;
    }
    else if (currentTask.type == TaskType::None || currentTask.type == TaskType::Idle)
    {
        for (auto *workstation : parentShip->getWorkstations())
        {
            if (workstation->inUse || workstation->bills.empty())
                continue;

            workstation->entityUsing = id;
            workstation->inUse = true;
            this->currentWorkstation = workstation;
            this->currentTask.type = TaskType::Work;
            currentTask.position = static_cast<sf::Vector2u>(sf::Vector2i(workstation->location) + dirOffsets[static_cast<std::uint8_t>(workstation->workStandDir)]);
        }
        if (currentTask.type == TaskType::None)
        {
            currentTask.type = TaskType::Idle;
            currentTask.position = {gridLocation.x + 1, gridLocation.y + 1};
        }

        targetLocation = currentTask.position;
    }
    else
    {
        taskAdded = false;
    }

    if (targetLocation == gridLocation)
    {
        switch (currentTask.type)
        {
            case TaskType::Idle:
                currentTask.type = TaskType::None;
                break;
            case TaskType::Work:
            {
                Bill& currentBill = currentWorkstation->bills.front();
                if (currentWorkstation->doWork())
                {
                    currentTask.type = TaskType::None;
                    currentWorkstation->inUse = false;
                }

                break;
            }
        }
        return;
    }

    if (m_steps.empty() || taskAdded)
    {
        updatePathfinding();

        while (m_steps.empty() && currentTask.type == TaskType::Idle && targetLocation.y < 31)
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
