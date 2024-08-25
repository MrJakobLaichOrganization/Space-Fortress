#include "pathfinding.hpp"

#include "block.hpp"

#include <limits>
#include <list>
#include <vector>
#include <cmath>
#include <ranges>

namespace
{
struct PathNode
{
    PathNode* parent;
    sf::Vector2i location;
    float cost = std::numeric_limits<float>::max();
    bool operator==(const PathNode& node) const
    {
        return node.location == location;
    }
    bool operator==(const sf::Vector2i& location) const
    {
        return this->location == location;
    }
};

float getTileValue(sf::Vector2i location, sf::Vector2i target)
{
    return std::sqrt(std::abs(static_cast<float>(location.x) - target.x)) +
           std::sqrt(std::abs(static_cast<float>(location.y) - target.y));
}

std::queue<sf::Vector2i> retracePath(const PathNode* start, const PathNode* end)
{
    const PathNode* tmpNode = end;
    std::vector<sf::Vector2i> tmpPath{};
    std::queue<sf::Vector2i> path{};

    while (tmpNode != start)
    {
        tmpPath.push_back(tmpNode->location - tmpNode->parent->location);
        tmpNode = tmpNode->parent;
    }
    for (auto it = tmpPath.rbegin(); it != tmpPath.rend(); it++)
    {
        path.push(*it);
    }
    
    return path;
}
} // namespace

std::queue<sf::Vector2i> generatePath(const class BlockGrid& grid,
                                      BlockGrid::Location start,
                                      BlockGrid::Location end,
                                      std::size_t maxSteps)
{
    std::deque<PathNode> openTiles{};
    std::list<PathNode> traveledTiles{};
    static const std::array<sf::Vector2i, 4> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
    auto isSolid = [&grid](sf::Vector2i loc)
    { return grid.getBlockArchetype(grid.getBlockData(sf::Vector2u(loc.x, loc.y)).blockAchetypeIdx).solid; };
    auto isValid = [&grid, &isSolid](sf::Vector2i loc) { return loc.x >= 0 && loc.y >= 0 && !isSolid(loc); };

    traveledTiles.push_back({nullptr, static_cast<sf::Vector2i>(start), 0});

    // populate the directional tiles
    for (const auto& dir : directions)
    {
        auto loc = static_cast<sf::Vector2i>(start) + dir;
        if (!isValid(loc))
            continue;

        openTiles.push_back(PathNode{&traveledTiles.front(), loc, getTileValue(loc, static_cast<sf::Vector2i>(end))});
    }

    auto location = openTiles.front().location;
    while (maxSteps-- && !openTiles.empty())
    {
        std::size_t minIdx = 0;
        for (std::size_t i = 1; i < openTiles.size(); ++i)
        {
            if (openTiles[i].cost < openTiles[minIdx].cost)
            {
                minIdx = i;
            }
        }

        traveledTiles.push_back(openTiles[minIdx]);
        location = openTiles[minIdx].location;
        openTiles.erase(openTiles.begin() + minIdx);

        if (location == static_cast<sf::Vector2i>(end))
        {
            return retracePath(&traveledTiles.front(), &traveledTiles.back());
        }

        for (const auto& dir : directions)
        {
            if (location.x == 0 && dir.x < 0 || location.y == 0 && dir.y < 0)
            {
                continue;
            }

            auto newLoc = location + dir;

            if (!isValid(newLoc) ||
                std::find(traveledTiles.begin(), traveledTiles.end(), newLoc) != traveledTiles.end())
            {
                continue;
            }
            if (std::find(openTiles.begin(), openTiles.end(), newLoc) != openTiles.end())
            {
                continue;
            }
            openTiles.push_back(PathNode{&traveledTiles.back(), newLoc, getTileValue(newLoc, static_cast<sf::Vector2i>(end))});
        }
    }

    return {};
}