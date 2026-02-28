#include <raygine/camera.hpp>

Camera2DEx::Camera2DEx(float pixelsPerMeter)
{
    scale = pixelsPerMeter;
    cam = {0};
    cam.offset = {400.0f, 300.0f};
    cam.rotation = 0.0f;
    cam.zoom = 1.0f;
}

void Camera2DEx::begin()
{
    BeginMode2D(cam);
}

void Camera2DEx::end()
{
    EndMode2D();
}

void Camera2DEx::follow(float x, float y)
{
    cam.target = {x, -y};
}

void Camera2DEx::setTarget(float x, float y)
{
    cam.target = {x, -y};
}

void Camera2DEx::setOffset(float x, float y)
{
    cam.offset = {x, y};
}

void Camera2DEx::setZoom(float z)
{
    cam.zoom = z;
}

void Camera2DEx::setScale(float s)
{
    scale = s;
}

float Camera2DEx::getScale() const 
{ 
    return scale; 
}

float Camera2DEx::worldToScreenY(float worldY) const
{
    return -worldY;
}
