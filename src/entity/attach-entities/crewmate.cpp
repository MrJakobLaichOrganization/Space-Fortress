#include "crewmate.hpp"

#include "entity/root-entities/ship.hpp"
#include "pathfinding.hpp"
#include "task/task.hpp"
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
}

void Crewmate::update(sf::Time deltaTime) // NOLINT
{
    if (!m_currentTask)
    {
        Ship& ship = *dynamic_cast<Ship*>(parent);

        if (!ship.tasks.empty())
        {
            m_currentTask = std::move(ship.tasks.front());
            ship.tasks.erase(ship.tasks.begin());
        }
        else
        {
            return;
        }
    }

    auto result = m_currentTask->resolve(*this);
    if (auto* error = std::get_if<TaskError>(&result))
    {
        m_currentTask.reset();
    }
    else if (std::get_if<TaskDone>(&result))
    {
        m_currentTask.reset();
    }
    else if (auto* action = std::get_if<Action>(&result))
    {
        std::visit([&](auto& act) { act.act(*this, deltaTime); }, *action);
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
