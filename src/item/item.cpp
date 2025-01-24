#include "item.hpp"

std::unordered_map<Item::Id, Item> ItemDatabase::items;
Item::Id ItemDatabase::nextItemID{};

Item::Id ItemDatabase::registerItem(std::string_view name, std::string_view description, std::uint32_t weight)
{
    items.insert(std::make_pair(nextItemID, Item(nextItemID, name, description, weight)));

    nextItemID++;

    return nextItemID - 1;
}
const Item* ItemDatabase::findItem(Item::Id id)
{
    auto iter = items.find(id);
    return iter == items.end() ? nullptr : &iter->second;
}