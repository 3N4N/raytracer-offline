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


#endif // _LIGHT_H_

