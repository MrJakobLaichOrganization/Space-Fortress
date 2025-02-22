#pragma once

#include "block.hpp"
#include "units.hpp"

#include <SFML/System/Vector2.hpp>

#include <deque>
#include <queue>
#include <vector>

#include <cstddef>

/// @brief Returns steps to take
/// @param grid - grid to use
/// @param start - beginning
/// @param end - destination
/// @param maxSteps - maximal number of steps which can be taken (100 by default)
std::vector<Location> generatePath(const class BlockGrid& grid, Location start, Location end, std::size_t maxSteps = 100);