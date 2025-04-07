#pragma once

#include "block.hpp"
#include "units.hpp"

#include <SFML/System/Time.hpp>

#include <memory>
#include <optional>
#include <utility>
#include <variant>
#include <vector>
#include <algorithm>

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

class Task
{
public:
    Crewmate* worker{};

    virtual ~Task()
    {
        assert(!worker);
    }

    virtual ActPtr start() = 0;
    virtual ~Task() = default;
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