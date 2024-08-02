 #include <cstdint>
#include <string>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <graphics/tilemap.hpp>

struct BlockArchetype
{
	std::string name;
	std::string description;
	// index of the tile in tilesheet
	std::uint32_t tilemapIdx;
};
struct BlockData
{
	// block archetype
	std::uint32_t blockAchetypeIdx;
};

class BlockGrid
{
  public:
	/// @param dimensions - grid size in tile count
	/// @param tilemap - optional parameter, leave nullptr if not tilemap will be linked
	BlockGrid(const sf::Vector2u &dimensions, Tilemap *tilemap = nullptr);

	/// @brief Sets block type in grid
	/// @param blockType - block archetype to set
	/// @param idx - index of the block in array
	void SetBlockType(std::uint32_t blockType, std::uint32_t idx);
	/// @brief 
	/// @param blockType - block archetype to set
	/// @param pos - position relative to the top left
	void SetBlockType(std::uint32_t blockType, const sf::Vector2u &pos);
	const BlockArchetype &GetBlockArchetype(std::uint32_t idx) const;
	const BlockArchetype &GetBlockArchetype(const sf::Vector2u &pos) const;

	const BlockData &GetBlockData(std::uint32_t idx) const;
	const BlockData &GetBlockData(const sf::Vector2u &pos) const;
	BlockData &GetBlockData(std::uint32_t idx);
	BlockData &GetBlockData(const sf::Vector2u &pos);

  private:
	std::uint32_t CalculateIndex(const sf::Vector2u &pos) const;

	Tilemap *tilemap;
	sf::Vector2u dims;
	std::vector<BlockArchetype> blockArchetypes;
	std::vector<BlockData> blockData;
};