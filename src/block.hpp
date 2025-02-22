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
    static constexpr std::uint32_t airIndex = 0;
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
    // floor archetype
    std::uint32_t floorAchetypeIdx{};

    class Entity* entity{};
};

using BlockArchetypeIndex = std::uint32_t;

class BlockGrid : protected OffsetGrid<BlockData>
{
public:
    using OffsetGrid::getBounds;
    using OffsetGrid::getMax;
    using OffsetGrid::getMin;
    using OffsetGrid::isValid;

    BlockGrid(Bounds bounds, TileRenderer* floorRenderer = nullptr, TileRenderer* wallRenderer = nullptr);

    void setBlockType(BlockArchetypeIndex blockType, Location loc, Direction dir = Direction::Up);
    void setBlockType(std::string_view archetypeName, Location loc, Direction dir = Direction::Up);

    void clearBlockType(Location loc);

    void setFloorType(BlockArchetypeIndex blockType, Location loc);
    void setFloorType(std::string_view archetypeName, Location loc);

    [[nodiscard]] const BlockArchetype& getBlockArchetype(Location loc) const;
    [[nodiscard]] BlockArchetypeIndex getBlockArchetypeIdx(std::string_view name) const;

    [[nodiscard]] const BlockArchetype& getFloorArchetype(Location loc) const;
    [[nodiscard]] BlockArchetypeIndex getFloorArchetypeIdx(std::string_view name) const;

    [[nodiscard]] const BlockData& getBlockData(Location loc) const;
    [[nodiscard]] BlockData& getBlockData(Location loc);

    static void loadArchetypes(cereal::JSONInputArchive& ar)
    {
        m_blockArchetypes.clear();
        ar(m_blockArchetypes); // NOLINT
    }

private:
    TileRenderer* m_mainRenderer = nullptr;
    TileRenderer* m_floorRenderer = nullptr;
    static std::vector<BlockArchetype> m_blockArchetypes; // NOLINT
};
