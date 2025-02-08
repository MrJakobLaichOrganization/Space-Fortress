#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include <cstdint>

struct Item
{
    using Id = std::uint32_t;

    Id id{};
    std::string name;
    std::string description;
    std::uint32_t weight{};

    Item() = default;
    Item(Id itemID, std::string_view itmName, std::string_view desc, std::uint32_t weigh) :
        id(itemID),
        name{itmName},
        description{desc},
        weight{weigh}
    {
    }
};

class ItemDatabase
{
private:
    static std::unordered_map<Item::Id, Item> items;
    static Item::Id nextItemID;

public:
    static Item::Id registerItem(std::string_view name, std::string_view description, std::uint32_t weight = 1);
    /// @brief Tries to find item
    /// @param id - id of the wanted item
    /// @return pointer to the item, nullptr if not found
    [[nodiscard]] static const Item* findItem(Item::Id id);
};