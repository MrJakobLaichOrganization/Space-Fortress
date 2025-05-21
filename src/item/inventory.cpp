#include "inventory.hpp"

#include "item/item.hpp"

#include <algorithm>
#include <ranges>

Inventory::Inventory(std::uint64_t capacity) : m_capacity{capacity}
{
}

[[nodiscard]] bool Inventory::contains(ItemType::Id typeId) const
{
    for (const auto& slot : m_items) // NOLINT
    {
        if (slot.item.type == typeId)
        {
            return true;
        }
    }
    return false;
}
int Inventory::getAmount(ItemType::Id typeId) const
{
    for (const auto& slot : m_items) // NOLINT
    {
        if (slot.item.type != typeId)
        {
            continue;
        }
        return slot.item.count;
    }
    return 0;
}
int Inventory::add(ItemType::Id typeId, int amt)
{
    const auto* type = ItemTypeDatabase::findItemType(typeId);
    if (!type)
    {
        return amt;
    }

    const std::uint32_t totalWeight = type->weight * amt;
    const std::uint64_t spaceLeft = m_capacity - m_currWeight;
    if (totalWeight > spaceLeft)
    {
        return amt;
    }

    const int toAdd = static_cast<int>(std::min<std::uint64_t>(amt, spaceLeft));

    for (auto& itmSlot : m_items)
    {
        if (itmSlot.item.type != typeId)
        {
            continue;
        }
        itmSlot.item.count += toAdd;
        return amt - toAdd;
    }

    m_items.push_back({Item{typeId, toAdd}});

    return amt - toAdd;
}
std::uint32_t Inventory::remove(ItemType::Id typeId, int amt)
{
    auto itmSlot = std::find_if(m_items.begin(),
                                m_items.end(),
                                [typeId](ItemSlot& slot) { return slot.item.type == typeId; });
    if (itmSlot == m_items.end())
    {
        return 0;
    }

    const std::uint32_t toRemove = std::min(itmSlot->item.count, amt);
    itmSlot->item.count -= toRemove;
    if (itmSlot->item.count == 0)
    {
        m_items.erase(itmSlot);
    }
    return toRemove;
}