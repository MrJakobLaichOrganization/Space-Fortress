#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_world.h"

namespace Box2dUtils
{

inline b2Body* findBodyAtPoint(b2World& world, b2Vec2 position)
{
    struct QueryCallback : public b2QueryCallback
    {
        b2Vec2 position{};
        b2Body* body{};

        bool ReportFixture(b2Fixture* fixture) override
        {
            if (fixture->TestPoint(position))
            {
                body = fixture->GetBody();
                return false;
            }

            return true;
        }
    };

    QueryCallback callback;
    callback.position = position;
    const b2Vec2 tolerance{0.000001f, 0.000001f};
    world.QueryAABB(&callback, b2AABB{position - tolerance, position + tolerance});

    return callback.body;
}

} // namespace Box2dUtils