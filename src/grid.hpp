#pragma once

#include "units.hpp"

#include <SFML/Graphics/Rect.hpp>

#include <vector>

#include <cstdint>

template <typename T>
class Grid
{
public:
    Grid(Dimension dimension) : m_dimension{dimension}, m_data(dimension.x * dimension.y)
    {
    }

    [[nodiscard]] Dimension getDimension() const
    {
        return m_dimension;
    }

    void set(Location loc, const T& t)
    {
        m_data[locationToIndex(loc)] = t;
    }

    [[nodiscard]] T& get(Location loc)
    {
        return get(locationToIndex(loc));
    }

    [[nodiscard]] const T& get(Location loc) const
    {
        return get(locationToIndex(loc));
    }

    [[nodiscard]] bool isValid(Location loc) const
    {
        return loc.x >= 0 && loc.x < m_dimension.x && loc.y >= 0 && loc.y < m_dimension.y;
    }

private:
    [[nodiscard]] Index getCount() const
    {
        return static_cast<Index>(m_data.size());
    }

    [[nodiscard]] Index locationToIndex(Location loc) const
    {
        return loc.y * m_dimension.x + loc.x;
    }

    [[nodiscard]] T& get(Index index)
    {
        return m_data[index];
    }

    [[nodiscard]] const T& get(Index index) const
    {
        return m_data[index];
    }

    Dimension m_dimension;

    std::vector<T> m_data;
};

template <typename T>
class OffsetGrid
{
public:
    OffsetGrid(Bounds bounds) : m_bounds{bounds}, m_data(bounds.size.x * bounds.size.y)
    {
    }

    [[nodiscard]] const Bounds& getBounds() const
    {
        return m_bounds;
    }

    [[nodiscard]] Location getMin() const
    {
        return m_bounds.position;
    }

    [[nodiscard]] Location getMax() const
    {
        return m_bounds.position + m_bounds.size;
    }

    void set(Location loc, const T& t)
    {
        m_data[locationToIndex(loc)] = t;
    }

    [[nodiscard]] T& get(Location loc)
    {
        return get(locationToIndex(loc));
    }

    [[nodiscard]] const T& get(Location loc) const
    {
        return get(locationToIndex(loc));
    }

    [[nodiscard]] bool isValid(Location loc) const
    {
        const auto index = locationToIndex(loc);
        return index >= 0 && index < getCount();
    }

    [[nodiscard]] Index locationToIndex(Location loc) const
    {
        return (loc.y - m_bounds.position.y) * m_bounds.size.x + (loc.x - m_bounds.position.x);
    }

private:
    [[nodiscard]] Index getCount() const
    {
        return static_cast<Index>(m_data.size());
    }

    [[nodiscard]] T& get(Index index)
    {
        return m_data[index];
    }

    [[nodiscard]] const T& get(Index index) const
    {
        return m_data[index];
    }

    Bounds m_bounds;

    std::vector<T> m_data;
};