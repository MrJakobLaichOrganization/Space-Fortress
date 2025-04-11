#pragma once

#include "block.hpp"
#include "units.hpp"

#include <SFML/System/Time.hpp>

#include <algorithm>
#include <memory>
#include <optional>
#include <utility>
#include <variant>
#include <vector>

class Crewmate;

class Act
{
public:
    enum class Status
    {
        Running,
        Success,
        Fail
    };

    virtual Status doAct(Crewmate& crewmate, sf::Time deltaTime) = 0;
    virtual ~Act() = default;
};

class MoveAct : public Act
{
public:
    Position targetPos;
    std::vector<Location> steps;

    MoveAct(Position targetPos) : targetPos{targetPos}
    {
    }
    Status doAct(Crewmate& crewmate, sf::Time deltaTime) override;
};

using ActPtr = std::unique_ptr<Act>;

class ActSequence : public Act
{
public:
    std::vector<ActPtr> acts;
    std::size_t actIndex{};

    template <typename... Args>
    ActSequence(Args&&... args)
    {
        (acts.push_back(std::forward<Args>(args)), ...);
    }

    ActSequence(ActSequence&&) = default;

    Status doAct(Crewmate& crewmate, sf::Time deltaTime) override
    {
        assert(actIndex < acts.size());

        auto& act = acts[actIndex];
        if (act)
        {
            auto result = act->doAct(crewmate, deltaTime);
            if (result != Status::Success)
            {
                return result;
            }

            act = nullptr;
            actIndex++;
        }

        if (actIndex >= acts.size())
        {
            return Status::Success;
        }
        else
        {
            return Status::Running;
        }
    }
};

class Task
{
public:
    Crewmate* worker{};

    virtual ~Task()
    {
        assert(!worker);
    }

    virtual ActPtr start() = 0;
};

class MoveTask : public Task
{
public:
    MoveTask(Position targetPos) : targetPos{targetPos}
    {
    }

    ActPtr start() override
    {
        return std::make_unique<MoveAct>(targetPos);
    }

    Position targetPos;
};