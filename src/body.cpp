#include <raygine/body.hpp>
#include <cmath>

// Default conversion parameters (static members)
float Body::defaultOriginX = 400.0f;
float Body::defaultOriginY = 300.0f;
float Body::defaultScale = 20.0f;

Body::Body(b2WorldId worldId, float x, float y, bool isDynamic, float w, float h)
{
    width = w;
    height = h;
    grounded = false;

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = {x, y};
    bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
    bodyId = b2CreateBody(worldId, &bodyDef);

    b2Polygon box = b2MakeBox(w / 2.0f, h / 2.0f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();

    if (isDynamic)
    {
        b2SurfaceMaterial mat = b2DefaultSurfaceMaterial();
        mat.friction = 0.5f;
        mat.restitution = 0.3f;
        shapeDef.material = mat;
        shapeDef.density = 1.0f;
    }

    shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);
}

Body::~Body()
{
    b2DestroyBody(bodyId);
}

b2BodyId Body::getId() const
{
    return bodyId;
}

void Body::setPosition(float x, float y)
{
    b2Body_SetTransform(bodyId, {x, y}, b2MakeRot(0.0f));
}

void Body::applyForce(float fx, float fy)
{
    b2Vec2 pos = b2Body_GetPosition(bodyId);
    b2Body_ApplyLinearImpulse(bodyId, {fx, fy}, pos, true);
}

void Body::setVelocity(float vx, float vy)
{
    b2Body_SetLinearVelocity(bodyId, {vx, vy});
}

b2Vec2 Body::getVelocity() const
{
    return b2Body_GetLinearVelocity(bodyId);
}

float Body::getX() const
{
    return b2Body_GetPosition(bodyId).x;
}

float Body::getY() const
{
    return b2Body_GetPosition(bodyId).y;
}

float Body::getWidth() const
{
    return width;
}

float Body::getHeight() const
{
    return height;
}

bool Body::isGrounded() const
{
    return grounded;
}

void Body::updateGrounded()
{
    b2Vec2 vel = b2Body_GetLinearVelocity(bodyId);

    if (fabs(vel.y) < 0.1f)
    {
        grounded = true;
    }
    else
    {
        grounded = false;
    }
}

void Body::moveLeft(float velocity)
{
    b2Vec2 currentVel = b2Body_GetLinearVelocity(bodyId);
    b2Body_SetLinearVelocity(bodyId, {-velocity, currentVel.y});
}

void Body::moveRight(float velocity)
{
    b2Vec2 currentVel = b2Body_GetLinearVelocity(bodyId);
    b2Body_SetLinearVelocity(bodyId, {velocity, currentVel.y});
}

void Body::moveUp(float force)
{
    if (grounded)
    {
        b2Vec2 pos = b2Body_GetPosition(bodyId);
        b2Body_ApplyLinearImpulse(bodyId, {0, force}, pos, true);
        grounded = false;
    }
}

void Body::moveDown(float force)
{
    b2Vec2 pos = b2Body_GetPosition(bodyId);
    b2Body_ApplyLinearImpulse(bodyId, {0, -force}, pos, true);
}

ScreenRect Body::toScreen(float originX, float originY, float scale) const
{
    ScreenRect rect;
    rect.x = originX + getX() * scale;
    rect.y = originY - getY() * scale;
    rect.width = static_cast<int32_t>(getWidth() * scale);
    rect.height = static_cast<int32_t>(getHeight() * scale);
    return rect;
}

ScreenRectF Body::toScreenF(float originX, float originY, float scale) const
{
    ScreenRectF rect;
    rect.width = getWidth() * scale;
    rect.height = getHeight() * scale;
    rect.x = originX + getX() * scale - rect.width * 0.5f;
    rect.y = originY - getY() * scale - rect.height * 0.5f;
    return rect;
}

void Body::setDefaultOrigin(float x, float y)
{
    defaultOriginX = x;
    defaultOriginY = y;
}

void Body::setDefaultScale(float scale)
{
    defaultScale = scale;
}

float Body::getDefaultOriginX()
{
    return defaultOriginX;
}

float Body::getDefaultOriginY()
{
    return defaultOriginY;
}

float Body::getDefaultScale()
{
    return defaultScale;
}
