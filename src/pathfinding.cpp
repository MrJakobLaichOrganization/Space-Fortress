#include "pathfinding.hpp"

#include "block.hpp"

#include <algorithm>
#include <limits>
#include <list>
#include <ranges>
#include <vector>

#include <cmath>

namespace
{
struct PathNode
{
    PathNode* parent;
    Location location;
    float cost = std::numeric_limits<Distance>::max();
    bool operator==(const PathNode& node) const
    {
        return node.location == location;
    }
    bool operator==(const Location& location) const
    {
        return this->location == location;
    }
};

float getTileValue(Location location, Location target)
{
    return std::sqrt(std::abs(static_cast<Distance>(location.x) - target.x)) +
           std::sqrt(std::abs(static_cast<Distance>(location.y) - target.y));
}

std::queue<Location> retracePath(const PathNode& start, const PathNode& end)
{
    const PathNode* tmpNode = &end;
    std::vector<Location> tmpPath{};
    std::queue<Location> path{};

    while (tmpNode != &start)
    {
        tmpPath.push_back(tmpNode->location - tmpNode->parent->location);
        tmpNode = tmpNode->parent;
    }
    // NOLINTBEGIN
    for (auto it = tmpPath.rbegin(); it != tmpPath.rend(); it++)
    {
        path.push(*it);
    }
    // NOLINTEND

    return path;
}

std::vector<Location> makePath(const PathNode& start, const PathNode& end)
{
    const PathNode* tmpNode = &end;
    std::vector<Location> path{};

    while (tmpNode != &start)
    {
        path.push_back(tmpNode->location);
        tmpNode = tmpNode->parent;
    }

    path.push_back(start.location);

    std::ranges::reverse(path);

    return path;
}
} // namespace

std::vector<Location> generatePath(const class BlockGrid& grid, Location start, Location end, std::size_t maxSteps)
{
    static const std::array<Location, 8> directions = {
        {{-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}}};
    std::deque<PathNode> openTiles{};
    std::list<PathNode> traveledTiles{};
    auto isSolid = [&grid](Location loc)
    {
        return grid.getBlockArchetype(loc).solid;
    };
    auto isValid = [&grid, &isSolid](Location loc)
    {
        return grid.isValid(loc) && !isSolid(loc);
    };

    // if end location is bad
    if (!isValid(end))
    {
        return {};
    }

    traveledTiles.push_back({nullptr, start, 0});

    // populate the directional tiles
    for (const auto& dir : directions)
    {
        auto loc = start + dir;
        if (!isValid(loc))
            continue;

        openTiles.push_back(PathNode{&traveledTiles.front(), loc, getTileValue(loc, end)});
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

        if (location == end)
        {
            return makePath(traveledTiles.front(), traveledTiles.back());
        }

        std::array<bool, directions.size()> directionValid{};

        for (int x = 0; x < directions.size(); x++)
        {
            directionValid[x] = isValid(location + directions[x]);
        }

        for (int x = 0; x < directions.size(); x++)
        {
            if (!directionValid[x])
            {
                continue;
            }

            if (x & 1 && (!directionValid[(x - 1 + directions.size()) % directions.size()] ||
                          !directionValid[(x + 1) % directions.size()]))
            {
                continue;
            }

            auto newLoc = location + directions[x];

            if (!isValid(newLoc) || std::find(traveledTiles.begin(), traveledTiles.end(), newLoc) != traveledTiles.end())
            {
                continue;
            }
            if (std::find(openTiles.begin(), openTiles.end(), newLoc) != openTiles.end())
            {
                continue;
            }

            openTiles.push_back(PathNode{&traveledTiles.back(), newLoc, getTileValue(newLoc, end)});
        }
    }

    return {};
}