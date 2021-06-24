#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include "vector.h"

class Object {
public:
    vector3d ref_point;
    double height, width, length;
    double color[3];
    double coeff[4]; // reflection coefficients
    int shine; // exponent term of specular component

    Object();
    ~Object();
    virtual void draw() = 0;
    void setColor(double, double, double);
    void setShine(int);
    void setCoeff(double, double, double, double);
};


#endif // _OBJECTS_H_
