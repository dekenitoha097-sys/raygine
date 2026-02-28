#ifndef WORLD
#define WORLD
#include <box2d/box2d.h>

class World
{
private:
    b2WorldDef worldDef;
    b2WorldId world;

public:
    World(float gravityX = 0.0f, float gravityY = -10.0f);
    ~World();
    void step(float timeStep);
    b2WorldId getId() const;

    // True if the body has at least one active physical contact.
    bool isBodyColliding(b2BodyId bodyId) const;

    // Backward-compatible overload: checks if bodyA touches bodyB.
    bool isBodyColliding(b2BodyId bodyA, b2BodyId bodyB) const;

    // True if two bodies are physically touching.
    bool areBodiesColliding(b2BodyId bodyA, b2BodyId bodyB) const;

    // Ground check based on contact normals (Y up in Box2D space).
    bool isBodyGrounded(b2BodyId bodyId, float minGroundNormalY = 0.5f) const;

    // Physics raycast in the world (independent from raylib helpers).
    b2RayResult rayCastClosest(
        b2Vec2 origin,
        b2Vec2 direction,
        float distance,
        b2QueryFilter filter = b2DefaultQueryFilter()) const;
};
#endif
