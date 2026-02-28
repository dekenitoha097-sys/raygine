#ifndef CURVE_GROUND
#define CURVE_GROUND

#include <box2d/box2d.h>
#include <raylib.h>
#include <vector>

// Curve ground built from a Box2D chain shape (one-sided)
class CurveGround
{
private:
    b2BodyId bodyId;
    b2ChainId chainId;
    std::vector<b2Vec2> curvePoints;

public:
    CurveGround(b2WorldId worldId, const std::vector<b2Vec2> &points,
                float friction = 0.9f, float restitution = 0.0f, bool loop = false);
    ~CurveGround();

    b2BodyId getBodyId() const;
    b2ChainId getChainId() const;
    const std::vector<b2Vec2> &getPoints() const;
    bool isValid() const;
    void draw(float scale, float originX, float originY, float thickness = 3.0f,
              Color color = {120, 220, 120, 255}) const;

    // Helper to build a sine curve
    static std::vector<b2Vec2> makeSinePoints(float xStart, float xEnd, float step,
                                              float amplitude, float frequency, float yOffset);
};

#endif
