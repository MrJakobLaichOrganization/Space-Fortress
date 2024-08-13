#include "pathfinding.hpp"

#include "block.hpp"

#include <limits>
#include <vector>
#include <list>
#include <cmath>


namespace
{
struct PathNode
{
    PathNode* parent;
    sf::Vector2i location;
    float cost = std::numeric_limits<float>::max();
    bool operator==(const PathNode& node)
    {
        return node.location == location;
    }
    bool operator==(const sf::Vector2i& location)
    {
        return this.location == location;
    }
};

float getTileValue(sf::Vector2i location, sf::Vector2i target)
{
    return std::sqrt(static_cast<float>(location.x) - target.x) + std::sqrt(static_cast<float>(location.y) - target.y);
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

std::queue<sf::Vector2i> generatePath(const class BlockGrid& grid, sf::Vector2i start, sf::Vector2i end, std::size_t maxSteps)
{
    std::deque<PathNode> openTiles{};
    std::list<PathNode> traveledTiles{};
    static const std::array<sf::Vector2i, 4> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
    auto isSolid = [&grid](sf::Vector2i pos)
    { return grid.getBlockArchetype(grid.getBlockData(sf::Vector2u(pos.x, pos.y)).blockAchetypeIdx).solid; };
    auto isValid = [&grid, &isSolid](sf::Vector2i pos) { return pos.x >= 0 && pos.y >= 0 && !isSolid(pos); };

    traveledTiles.push_back({nullptr, start, 0});

    // populate the directional tiles
    for (const auto& dir : directions)
    {
        auto location = start + dir;
        if (!isValid(location))
            continue;

        openTiles.push_back(PathNode{&traveledTiles.front(), location, getTileValue(location, end)});
    }

    auto location = openTiles.front().location;
    while (maxSteps-- && openTiles.size())
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

        if (location == end)
        {
            return retracePath(&traveledTiles.front(), &traveledTiles.back());
        }

        for (const auto& dir : directions)
        {
            if (location.x == 0 && dir.x < 0 || location.y == 0 && dir.y < 0)
            {
                continue;
            }
            if (!isValid(location + dir) ||
                std::find(traveledTiles.begin(), traveledTiles.end(), location + dir) != traveledTiles.end())
            {
                continue;
            }
            if (std::find(openTiles.begin(), openTiles.end(), location + dir) != openTiles.end())
            {
                continue;
            }
            openTiles.push_back(PathNode{&traveledTiles.back(), {location + dir}, getTileValue(location + dir, end)});
        }
    }

    return {};
}
