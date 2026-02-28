#include <raygine/curve.hpp>

#include <cmath>

CurveGround::CurveGround(b2WorldId worldId, const std::vector<b2Vec2>& points,
                         float friction, float restitution, bool loop)
{
    bodyId = b2_nullBodyId;
    chainId = b2_nullChainId;
    curvePoints = points;

    if (curvePoints.size() < 4)
    {
        return;
    }

    b2BodyDef groundDef = b2DefaultBodyDef();
    groundDef.type = b2_staticBody;
    bodyId = b2CreateBody(worldId, &groundDef);

    b2SurfaceMaterial mat = b2DefaultSurfaceMaterial();
    mat.friction = friction;
    mat.restitution = restitution;

    b2ChainDef chainDef = b2DefaultChainDef();
    chainDef.points = curvePoints.data();
    chainDef.count = static_cast<int>(curvePoints.size());
    chainDef.materials = &mat;
    chainDef.materialCount = 1;
    chainDef.isLoop = loop;

    chainId = b2CreateChain(bodyId, &chainDef);
}

CurveGround::~CurveGround()
{
    if (b2Chain_IsValid(chainId))
    {
        b2DestroyChain(chainId);
    }
    if (b2Body_IsValid(bodyId))
    {
        b2DestroyBody(bodyId);
    }
}

b2BodyId CurveGround::getBodyId() const
{
    return bodyId;
}

b2ChainId CurveGround::getChainId() const
{
    return chainId;
}

const std::vector<b2Vec2>& CurveGround::getPoints() const
{
    return curvePoints;
}

bool CurveGround::isValid() const
{
    return b2Chain_IsValid(chainId) && b2Body_IsValid(bodyId);
}

void CurveGround::draw(float scale, float originX, float originY, float thickness, Color color) const
{
    if (curvePoints.size() < 2)
    {
        return;
    }

    for (size_t i = 0; i + 1 < curvePoints.size(); ++i)
    {
        Vector2 a = {originX + curvePoints[i].x * scale, originY - curvePoints[i].y * scale};
        Vector2 b = {originX + curvePoints[i + 1].x * scale, originY - curvePoints[i + 1].y * scale};
        DrawLineEx(a, b, thickness, color);
    }
}

std::vector<b2Vec2> CurveGround::makeSinePoints(float xStart, float xEnd, float step,
                                                float amplitude, float frequency, float yOffset)
{
    std::vector<b2Vec2> points;
    if (step == 0.0f)
    {
        return points;
    }

    float actualStep = step;
    if (xEnd < xStart && step > 0.0f)
    {
        actualStep = -step;
    }
    else if (xEnd > xStart && step < 0.0f)
    {
        actualStep = -step;
    }

    for (float x = xStart;
         (actualStep > 0.0f) ? (x <= xEnd) : (x >= xEnd);
         x += actualStep)
    {
        float y = std::sinf(x * frequency) * amplitude + yOffset;
        points.push_back({x, y});
    }

    return points;
}
