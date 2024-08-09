#pragma once
#include <SFML/System/Vector2.hpp>

#include <graphics/tilemap.hpp>
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
    std::uint32_t blockAchetypeIdx;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(blockAchetypeIdx);
    }
};

class BlockGrid
{
public:
    /// @param dimensions - grid size in tile count
    /// @param tilemap - optional parameter, leave nullptr if not tilemap will be linked
    BlockGrid(const sf::Vector2u& dimensions, Tilemap* tilemap = nullptr);

    /// @brief Sets block type in grid
    /// @param blockType - block archetype to set
    /// @param idx - index of the block in array
    void setBlockType(std::uint32_t blockType, std::uint32_t idx);
    /// @brief
    /// @param blockType - block archetype to set
    /// @param pos - position relative to the top left
    void setBlockType(std::uint32_t blockType, const sf::Vector2u& pos);
    /// @brief
    /// @param blockType - block archetype name to set
    /// @param pos - position relative to the top left
    void setBlockType(std::string_view archetypeName, const sf::Vector2u& pos);
    [[nodiscard]] const BlockArchetype& getBlockArchetype(std::uint32_t idx) const;
    [[nodiscard]] const BlockArchetype& getBlockArchetype(const sf::Vector2u& pos) const;
    [[nodiscard]] std::uint32_t getBlockArchetypeIdx(std::string_view name) const;

    [[nodiscard]] const BlockData& getBlockData(std::uint32_t idx) const;
    [[nodiscard]] const BlockData& getBlockData(const sf::Vector2u& pos) const;
    [[nodiscard]] BlockData& getBlockData(std::uint32_t idx);
    [[nodiscard]] BlockData& getBlockData(const sf::Vector2u& pos);

    sf::Vector2u dims;

    // Doesnt deal with tilemap, you gotta pass it yourself
    template <class Archive>
    void save(Archive& ar) const
    {
        ar(dims.x, dims.y, m_blockData);
    }
    template <class Archive>
    void load(Archive& ar)
    {
        m_blockData.clear();
        ar(dims.x, dims.y, m_blockData);
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

    [[nodiscard]] std::uint32_t getBlockIdx(sf::Vector2u pos)
    {
        return pos.y * m_tilemap->getTilesetDims().x + pos.x;
    }
    [[nodiscard]] std::uint32_t calculateIndex(const sf::Vector2u& pos) const;

    Tilemap* m_tilemap = nullptr;
    static std::vector<BlockArchetype> m_blockArchetypes;
    std::vector<BlockData> m_blockData;
};
