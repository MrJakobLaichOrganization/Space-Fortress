#pragma once

#include "item/item.hpp"

#include <vector>

struct ItemSlot
{
    Item::Id item;
    std::uint32_t count;
};
class Inventory
{
public:
    Inventory(std::uint64_t capacity);

    [[nodiscard]] bool contains(Item::Id itemID) const;
    /// @brief Gets the amount of items of the type in inventory
    /// @param itemID id of the item
    /// @return amount of items in inventory
    std::uint32_t getAmount(Item::Id itemID) const;
    /// @brief Adds items to inventory
    /// @param itemID - ID of the item to add
    /// @param amt - amount of items to add
    /// @return number of items which couldn't fit (weren't added)
    std::uint32_t add(Item::Id itemID, std::uint32_t amt = 1);
    /// @brief Adds items to inventory
    /// @param itemID - ID of the item to add
    /// @param amt - amount of items to add
    /// @return number of items removed (less than amount if not enough of them)
    std::uint32_t remove(Item::Id itemID, std::uint32_t amt = 1);

private:
    std::uint64_t m_capacity; // Weight capacity
    std::uint64_t m_currWeight{};
    std::vector<ItemSlot> m_items;
};