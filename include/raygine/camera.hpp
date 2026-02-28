#ifndef CAMERA
#define CAMERA

#include <raylib.h>

// Simple camera for top-down / platformer games
// Uses Box2D coordinates (Y+ is UP) and converts to screen (Y+ is DOWN)
class Camera2DEx
{
private:
    Camera2D cam;
    float scale; // pixels per meter

public:
    Camera2DEx(float pixelsPerMeter = 20.0f);

    void begin();
    void end();

    // Follow a body - camera tracks the object
    void follow(float x, float y);
    void setTarget(float x, float y);
    void setOffset(float x, float y);
    void setZoom(float z);
    void setScale(float s);
    float getScale() const;

    // Convert world Y to screen Y (inverts the axis)
    float worldToScreenY(float worldY) const;
};

#endif
