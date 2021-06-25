#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vector.h"

class Light {
public:
    vector3d pos;
    double color[3];

    Light() { }
    Light(double x, double y, double z,
          double r, double g, double b);
    ~Light() { }
    void setPosition(double x, double y, double z);
    void setColor(double r, double g, double b);
};

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
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

#endif // _LIGHT_H_
