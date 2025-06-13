#include "attach-entity.hpp"
#include "root-entity.hpp"

AttachEntity::~AttachEntity()
{
    if (parent)
    {
        auto it = std::ranges::find(parent->children, this);
        assert(it != parent->children.end());
        parent->children.erase(it);
        parent = nullptr;
    }
}