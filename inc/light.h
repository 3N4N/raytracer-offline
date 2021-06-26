#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vector.h"
#include "color.h"


class Light {
public:
    vec3 pos;
    Color color;

    Light() { }
    Light(double x, double y, double z,
          double r, double g, double b);
    ~Light() { }
    void setPosition(double x, double y, double z);
    void setColor(double r, double g, double b);
    void draw() const;
};


#endif // _LIGHT_H_
