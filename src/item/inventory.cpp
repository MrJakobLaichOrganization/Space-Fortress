#include "inventory.hpp"

#include "item/item.hpp"

Inventory::Inventory(std::uint64_t capacity) : m_capacity{capacity}
{
}

[[nodiscard]] bool Inventory::contains(Item::Id itemID) const
{
    for (auto& slot : m_items)
    {
        if (slot.item == itemID)
        {
            return true;
        }
    }
    return false;
}
std::uint32_t Inventory::getAmount(Item::Id itemID) const
{
    for (auto& slot : m_items)
    {
        if (slot.item != itemID)
        {
            continue;
        }
        return slot.count;
    }
    return 0;
}
std::uint32_t Inventory::add(Item::Id itemID, std::uint32_t amt)
{
    auto* itm = ItemDatabase::findItem(itemID);
    if (!itm)
    {
        return amt;
    }

    std::uint32_t totalWeight = itm->weight * amt;
    std::uint64_t spaceLeft = m_capacity - m_currWeight;
    if (totalWeight > spaceLeft)
    {
        return amt;
    }

    std::uint32_t toAdd = static_cast<std::uint32_t>(std::min<std::uint64_t>(amt, spaceLeft));

    for (auto& itmSlot : m_items)
    {
        if (itmSlot.item != itemID)
        {
            continue;
        }
        itmSlot.count += toAdd;
        return amt - toAdd;
    }

    m_items.push_back({itemID, toAdd});

    return amt - toAdd;
}
std::uint32_t Inventory::remove(Item::Id itemID, std::uint32_t amt)
{
    auto itmSlot = std::find_if(m_items.begin(), m_items.end(), [itemID](ItemSlot& slot) { return slot.item == itemID; });
    if (itmSlot == m_items.end())
    {
        return 0;
    }

    std::uint32_t toRemove = std::min(itmSlot->count, amt);
    itmSlot->count -= toRemove;
    if (itmSlot->count == 0)
    {
        m_items.erase(itmSlot);
    }
    return toRemove;
}