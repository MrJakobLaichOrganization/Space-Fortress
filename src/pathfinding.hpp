#pragma once

#include <SFML/System/Vector2.hpp>

#include <queue>
#include <deque>
#include <cstddef>
#include <vector>

std::queue<sf::Vector2i> generatePath(const class BlockGrid& grid, sf::Vector2i start, sf::Vector2i end, std::size_t maxSteps = 100);