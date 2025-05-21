#include "item.hpp"

std::unordered_map<ItemType::Id, ItemType> ItemTypeDatabase::itemTypes;
ItemType::Id ItemTypeDatabase::nextItemTypeID{};

ItemType::Id ItemTypeDatabase::registerItemType(std::string_view name, std::string_view description, std::uint32_t weight)
{
    itemTypes.insert(std::make_pair(nextItemTypeID, ItemType(nextItemTypeID, name, description, weight)));

    nextItemTypeID++;

    return nextItemTypeID - 1;
}
const ItemType* ItemTypeDatabase::findItemType(ItemType::Id id)
{
    auto iter = itemTypes.find(id);
    return iter == itemTypes.end() ? nullptr : &iter->second;
}