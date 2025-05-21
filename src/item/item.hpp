#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include <cstdint>

struct ItemType
{
    using Id = std::uint32_t;

    Id id{};
    std::string name;
    std::string description;
    std::uint32_t weight{};

    ItemType() = default;
    ItemType(Id itemID, std::string_view itmName, std::string_view desc, std::uint32_t weigh) :
        id(itemID),
        name{itmName},
        description{desc},
        weight{weigh}
    {
    }
};

struct Item
{
    ItemType::Id type{};
    int count{};
};

class ItemTypeDatabase
{
private:
    static std::unordered_map<ItemType::Id, ItemType> itemTypes;
    static ItemType::Id nextItemTypeID;

public:
    static ItemType::Id registerItemType(std::string_view name, std::string_view description, std::uint32_t weight = 1);
    /// @brief Tries to find item
    /// @param id - id of the wanted item
    /// @return pointer to the item, nullptr if not found
    [[nodiscard]] static const ItemType* findItemType(ItemType::Id id);
};