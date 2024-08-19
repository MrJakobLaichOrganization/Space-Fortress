#pragma once

#include <cstddef>
#include <vector>

template <typename T, typename LocationT = sf::Vector2u, typename IndexT = std::uint32_t>
class Grid
{
public:
    using Location = LocationT;
    using Dimension = LocationT;
    using Index = IndexT;

    Grid(Dimension dimension) : m_dimension{dimension}, m_data(dimension.x * dimension.y)
    {
    }

    [[nodiscard]] Dimension getDimension() const
    {
        return m_dimension;
    }

    [[nodiscard]] std::size_t getCount() const
    {
        return m_data.size();
    }

    [[nodiscard]] Index locationToIndex(Location loc) const
    {
        return loc.y * m_dimension.x + loc.x;
    }

    void set(Location loc, const T& t)
    {
        m_data[locationToIndex(loc)] = t;
    }

    [[nodiscard]] T& get(Location loc)
    {
        return m_data[locationToIndex(loc)];
    }

    [[nodiscard]] const T& get(Location loc) const
    {
        return m_data[locationToIndex(loc)];
    }

    [[nodiscard]] T& get(Index index)
    {
        return m_data[index];
    }

    [[nodiscard]] const T& get(Index index) const
    {
        return m_data[index];
    }

    template <class Archive>
    void save(Archive& ar) const
    {
        ar(m_dimension.x, m_dimension.y, m_data);
    }
    template <class Archive>
    void load(Archive& ar)
    {
        m_data.clear();
        ar(m_dimension.x, m_dimension.y, m_data);
    }

    //todo: resize

private:
    Dimension m_dimension;

    std::vector<T> m_data;
};