#pragma once

class BuildingAct : public Act
{
public:
    Location location;
    float duration{1.5f};

    BuildingAct(Location location, float duration);

    Status doAct(Crewmate& crewmate, sf::Time deltaTime) override;
};

class BuildingTask : public Task
{
public:
    Location location;
    float duration;

    BuildingTask(Location location, float duration) : location{location}, duration{duration}
    {
    }

    ActPtr start() override;
};