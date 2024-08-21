#pragma once

#include <SFML/System/Vector2.hpp>

#include <deque>
#include <queue>
#include <vector>
#include <cstddef>

#include "units.hpp"
#include "block.hpp"

/// @brief Returns steps to take
/// @param grid - grid to use
/// @param start - beginning
/// @param end - destination
/// @param maxSteps - maximal number of steps which can be taken (100 by default)
std::queue<sf::Vector2i> generatePath(const class BlockGrid& grid,
                                      BlockGrid::Location start,
                                      BlockGrid::Location end,
                                      std::size_t maxSteps = 100);