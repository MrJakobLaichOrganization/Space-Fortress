#include "pathfinding.hpp"

#include "block.hpp"

#include <limits>
#include <vector>

#include <cmath>

namespace
{
struct PathNode
{
    PathNode* parent;
    sf::Vector2i pos;
    float cost = std::numeric_limits<float>::max();

    bool operator==(const PathNode& node)
    {
        return node.pos == pos;
    }
    bool operator==(const sf::Vector2i& position)
    {
        return pos == position;
    }
};

float getTileValue(sf::Vector2i pos, sf::Vector2i target)
{
    return std::sqrtf(pos.x - target.x) + std::sqrtf(pos.y - target.y);
}

std::queue<sf::Vector2i> retracePath(const PathNode* start, const PathNode* end)
{
    const PathNode* tmpNode = end;
    std::vector<sf::Vector2i> tmpPath{};
    std::queue<sf::Vector2i> path{};

    while (tmpNode != start)
    {
        tmpPath.push_back(tmpNode->pos - tmpNode->parent->pos);
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
        auto position = start + dir;
        if (!isValid(position))
            continue;

        openTiles.push_back(PathNode{&traveledTiles.front(), position, getTileValue(position, end)});
    }

    auto pos = openTiles.front().pos;
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
        pos = openTiles[minIdx].pos;
        openTiles.erase(openTiles.begin() + minIdx);

        if (pos == end)
        {
            return retracePath(&traveledTiles.front(), &traveledTiles.back());
        }

        for (const auto& dir : directions)
        {
            if (pos.x == 0 && dir.x < 0 || pos.y == 0 && dir.y < 0)
            {
                continue;
            }
            if (!isValid(pos + dir) ||
                std::find(traveledTiles.begin(), traveledTiles.end(), pos + dir) != traveledTiles.end())
            {
                continue;
            }
            if (std::find(openTiles.begin(), openTiles.end(), pos + dir) != openTiles.end())
            {
                continue;
            }
            openTiles.push_back(PathNode{&traveledTiles.back(), {pos + dir}, getTileValue(pos + dir, end)});
        }
    }

    return {};
}