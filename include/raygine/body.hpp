#ifndef BODY
#define BODY
#include <box2d/box2d.h>
#include <cstdint>

// Screen rectangle for coordinate conversion
struct ScreenRect
{
    float x;
    float y;
    int32_t width;
    int32_t height;
};

// Float rectangle for rendering systems (sprites, custom renderer, etc.)
struct ScreenRectF
{
    float x;
    float y;
    float width;
    float height;
};

class Body
{
private:
    b2BodyId bodyId;
    b2ShapeId shapeId;
    float width;
    float height;
    bool grounded;

public:
    // Static default conversion parameters
    static float defaultOriginX;
    static float defaultOriginY;
    static float defaultScale;

    Body(b2WorldId worldId, float x, float y, bool isDynamic = true, 
         float w = 1.0f, float h = 1.0f);
    ~Body();
    b2BodyId getId() const;
    void setPosition(float x, float y);
    void applyForce(float fx, float fy);
    void setVelocity(float vx, float vy);
    b2Vec2 getVelocity() const;
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    bool isGrounded() const;
    void updateGrounded();
    
    // Movement methods
    void moveLeft(float velocity);
    void moveRight(float velocity);
    void moveUp(float force);
    void moveDown(float force);
    
    // Coordinate conversion with default values
    ScreenRect toScreen(float originX = 400.0f, float originY = 300.0f, float scale = 20.0f) const;
    ScreenRectF toScreenF(float originX = 400.0f, float originY = 300.0f, float scale = 20.0f) const;
    
    // Set default conversion parameters for all bodies
    static void setDefaultOrigin(float x, float y);
    static void setDefaultScale(float scale);
    static float getDefaultOriginX();
    static float getDefaultOriginY();
    static float getDefaultScale();
};

#endif
