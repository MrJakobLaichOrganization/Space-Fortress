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
    BlockGrid::Location location;
    float cost = std::numeric_limits<float>::max();
    bool operator==(const PathNode& node) const
    {
        return node.location == location;
    }
    bool operator==(const BlockGrid::Location& location) const
    {
        return this->location == location;
    }
    bool operator==(const sf::Vector2i& location) const
    {
        return this->location == BlockGrid::Location(location);
    }
};

float getTileValue(sf::Vector2i location, sf::Vector2i target)
{
    return std::sqrt(std::abs(static_cast<float>(location.x) - target.x)) +
           std::sqrt(std::abs(static_cast<float>(location.y) - target.y));
}

std::queue<BlockGrid::Location> retracePath(const PathNode& start, const PathNode& end)
{
    const PathNode* tmpNode = &end;
    std::vector<BlockGrid::Location> tmpPath{};
    std::queue<BlockGrid::Location> path{};

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

std::vector<BlockGrid::Location> makePath(const PathNode& start, const PathNode& end)
{
    const PathNode* tmpNode = &end;
    std::vector<BlockGrid::Location> path{};

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

std::vector<BlockGrid::Location> generatePath(const class BlockGrid& grid,
                                              BlockGrid::Location start,
                                              BlockGrid::Location end,
                                              std::size_t maxSteps)
{
    static const std::array<sf::Vector2i, 4> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
    std::deque<PathNode> openTiles{};
    std::list<PathNode> traveledTiles{};
    auto isSolid = [&grid](sf::Vector2i loc)
    {
        return grid.getBlockArchetype(sf::Vector2u(loc.x, loc.y)).solid;
    };
    auto isValid = [&grid, &isSolid](sf::Vector2i loc)
    {
        return loc.x >= 0 && loc.y >= 0 && !isSolid(loc);
    };

    // if end location is bad
    if (!isValid(sf::Vector2i(end.x, end.y)))
    {
        return {};
    }

    traveledTiles.push_back({nullptr, static_cast<BlockGrid::Location>(start), 0});

    // populate the directional tiles
    for (const auto& dir : directions)
    {
        auto loc = static_cast<sf::Vector2i>(start) + dir;
        if (!isValid(loc))
            continue;

        openTiles.push_back(PathNode{&traveledTiles.front(),
                                     static_cast<BlockGrid::Location>(loc),
                                     getTileValue(loc, static_cast<sf::Vector2i>(end))});
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

        if (location == static_cast<BlockGrid::Location>(end))
        {
            return makePath(traveledTiles.front(), traveledTiles.back());
        }

        for (const auto& dir : directions)
        {
            if (location.x == 0 && dir.x < 0 || location.y == 0 && dir.y < 0)
            {
                continue;
            }

            auto newLoc = static_cast<sf::Vector2i>(location) + dir;

            if (!isValid(newLoc) || std::find(traveledTiles.begin(), traveledTiles.end(), newLoc) != traveledTiles.end())
            {
                continue;
            }
            if (std::find(openTiles.begin(), openTiles.end(), newLoc) != openTiles.end())
            {
                continue;
            }

            openTiles.push_back(PathNode{&traveledTiles.back(),
                                         static_cast<BlockGrid::Location>(newLoc),
                                         getTileValue(newLoc, static_cast<sf::Vector2i>(end))});
        }
    }

    return {};
}