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
    Object(double, double, double, double, double, double,
           int, double, double, double, double);
    Object(vector3d, double, double, double,
           int, double, double, double, double);
    Object(double, double, double, int, double, double, double, double);
    ~Object();
    virtual void draw() = 0;
    void setColor(double, double, double);
    void setShine(int);
    void setCoeff(double, double, double, double);
};

class Sphere: Object {
public:
    double radius;

    Sphere(): Object() { }
    Sphere(double, double, double, double, double, double, double,
           int, double, double, double, double);
    Sphere(vector3d, double, double, double, double,
           int, double, double, double, double);
};

class Triangle: Object {
public:
    vector3d p1, p2, p3;

    Triangle(): Object() { }
    Triangle(double,double,double,double,double,double,double,double,double,
             double, double, double, int, double, double, double, double);
};


#endif // _OBJECTS_H_
