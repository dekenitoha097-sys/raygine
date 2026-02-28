#include <raygine/world.hpp>
#include <cmath>
#include <vector>

World::World(float gravityX, float gravityY){
    worldDef = b2DefaultWorldDef();
    worldDef.gravity = {gravityX, gravityY};
    world = b2CreateWorld(&worldDef);
}

World::~World(){
    b2DestroyWorld(world);
}

void World::step(float timeStep){
    b2World_Step(world, timeStep, 1);
}

b2WorldId World::getId() const{
    return world;
}

bool World::isBodyColliding(b2BodyId bodyId) const{
    if (!b2Body_IsValid(bodyId))
    {
        return false;
    }

    const int capacity = b2Body_GetContactCapacity(bodyId);
    if (capacity <= 0)
    {
        return false;
    }

    std::vector<b2ContactData> contacts(static_cast<size_t>(capacity));
    const int count = b2Body_GetContactData(bodyId, contacts.data(), capacity);
    return count > 0;
}

bool World::isBodyColliding(b2BodyId bodyA, b2BodyId bodyB) const{
    return areBodiesColliding(bodyA, bodyB);
}

bool World::areBodiesColliding(b2BodyId bodyA, b2BodyId bodyB) const{
    if (!b2Body_IsValid(bodyA) || !b2Body_IsValid(bodyB))
    {
        return false;
    }

    const int capacity = b2Body_GetContactCapacity(bodyA);
    if (capacity <= 0)
    {
        return false;
    }

    std::vector<b2ContactData> contacts(static_cast<size_t>(capacity));
    const int count = b2Body_GetContactData(bodyA, contacts.data(), capacity);

    for (int i = 0; i < count; ++i)
    {
        const b2BodyId contactBodyA = b2Shape_GetBody(contacts[i].shapeIdA);
        const b2BodyId contactBodyB = b2Shape_GetBody(contacts[i].shapeIdB);

        const bool directMatch = B2_ID_EQUALS(contactBodyA, bodyA) && B2_ID_EQUALS(contactBodyB, bodyB);
        const bool reverseMatch = B2_ID_EQUALS(contactBodyA, bodyB) && B2_ID_EQUALS(contactBodyB, bodyA);
        if (directMatch || reverseMatch)
        {
            return true;
        }
    }

    return false;
}

bool World::isBodyGrounded(b2BodyId bodyId, float minGroundNormalY) const{
    if (!b2Body_IsValid(bodyId))
    {
        return false;
    }

    const int capacity = b2Body_GetContactCapacity(bodyId);
    if (capacity <= 0)
    {
        return false;
    }

    std::vector<b2ContactData> contacts(static_cast<size_t>(capacity));
    const int count = b2Body_GetContactData(bodyId, contacts.data(), capacity);

    for (int i = 0; i < count; ++i)
    {
        if (contacts[i].manifold.pointCount <= 0)
        {
            continue;
        }

        const b2BodyId contactBodyA = b2Shape_GetBody(contacts[i].shapeIdA);
        const b2BodyId contactBodyB = b2Shape_GetBody(contacts[i].shapeIdB);

        float upNormalForBody = 0.0f;
        if (B2_ID_EQUALS(contactBodyA, bodyId))
        {
            upNormalForBody = -contacts[i].manifold.normal.y;
        }
        else if (B2_ID_EQUALS(contactBodyB, bodyId))
        {
            upNormalForBody = contacts[i].manifold.normal.y;
        }
        else
        {
            continue;
        }

        if (upNormalForBody >= minGroundNormalY)
        {
            return true;
        }
    }

    return false;
}

b2RayResult World::rayCastClosest(b2Vec2 origin, b2Vec2 direction, float distance, b2QueryFilter filter) const{
    b2RayResult result = {};

    if (distance <= 0.0f)
    {
        return result;
    }

    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length <= 0.0f)
    {
        return result;
    }

    const b2Vec2 translation = {
        direction.x / length * distance,
        direction.y / length * distance
    };

    return b2World_CastRayClosest(world, origin, translation, filter);
}
