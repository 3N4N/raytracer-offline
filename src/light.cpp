#include "light.h"

Light::Light(double x, double y, double z,
             double r, double g, double b)
{
    setPosition(x, y, z);
    setColor(r, g, b);
}

void Light::setPosition(double x, double y, double z)
{
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;
}

void Light::setColor(double r, double g, double b)
{
    color.r = r;
    color.g = g;
    color.b = b;
}

void Light::draw() { }

