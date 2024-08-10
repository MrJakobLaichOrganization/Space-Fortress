#pragma once

#include "graphics/tilemap.hpp"
#include "grid.hpp"

#include <SFML/System/Vector2.hpp>

#include <string>
#include <string_view>
#include <vector>

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cstdint>

struct BlockArchetype
{
    std::string name;
    std::string description;
    // index of the tile in tilesheet
    std::uint32_t tilemapIdx{};
    bool solid = true;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(name, description, tilemapIdx, solid);
    }
};
struct BlockData
{
    // block archetype
    std::uint32_t blockAchetypeIdx{};

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(blockAchetypeIdx);
    }
};

using BlockArchetypeIndex = std::uint32_t;

class BlockGrid : protected Grid<BlockData>
{
public:
    using Location = Grid::Location;
    using Index = Grid::Index;

    using Grid::getCount;
    using Grid::getDimension;

    /// @param dimensions - grid size in tile count
    /// @param tilemap - optional parameter, leave nullptr if not tilemap will be linked
    BlockGrid(sf::Vector2u dimensions, Tilemap* tilemap = nullptr);

    /// @brief Sets block type in grid
    /// @param blockType - block archetype to set
    /// @param idx - index of the block in array
    void setBlockType(BlockArchetypeIndex blockType, Index idx);
    /// @brief
    /// @param blockType - block archetype to set
    /// @param pos - position relative to the top left
    void setBlockType(BlockArchetypeIndex blockType, Location pos);
    /// @brief
    /// @param blockType - block archetype name to set
    /// @param pos - position relative to the top left
    void setBlockType(std::string_view archetypeName, Location pos);
    [[nodiscard]] const BlockArchetype& getBlockArchetype(Index idx) const;
    [[nodiscard]] const BlockArchetype& getBlockArchetype(Location pos) const;
    [[nodiscard]] BlockArchetypeIndex getBlockArchetypeIdx(std::string_view name) const;

    [[nodiscard]] const BlockData& getBlockData(Index idx) const;
    [[nodiscard]] const BlockData& getBlockData(Location pos) const;
    [[nodiscard]] BlockData& getBlockData(Index idx);
    [[nodiscard]] BlockData& getBlockData(Location pos);

    // Doesnt deal with tilemap, you gotta pass it yourself
    template <class Archive>
    void save(Archive& ar) const
    {
        Grid::save(ar);
    }
    template <class Archive>
    void load(Archive& ar)
    {
        Grid::load(ar);
    }

    template <class Archive>
    [[nodiscard]] static BlockGrid loadFromFile(Archive& ar, Tilemap* tilemap = nullptr)
    {
        BlockGrid returnVal;

        ar(returnVal);
        returnVal.m_tilemap = tilemap;

        return returnVal;
    }
    static void loadArchetypes(cereal::JSONInputArchive& ar)
    {
        m_blockArchetypes.clear();
        ar(m_blockArchetypes);
    }

private:
    BlockGrid() = default;

    Tilemap* m_tilemap = nullptr;
    static std::vector<BlockArchetype> m_blockArchetypes;
};
