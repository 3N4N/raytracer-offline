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
    Object(double x, double y, double z,
           double r, double g, double b,
           int shine, double ambient, double diffuse,
           double specular, double recursive);
    Object(vector3d ref, double r, double g, double b,
           int shine, double ambient, double diffuse,
           double specular, double recursive);
    Object(double r, double g, double b,
           int shine, double ambient, double diffuse,
           double specular, double recursive);
    ~Object();
    // virtual void draw() = 0;
    void setColor(double, double, double);
    void setShine(int);
    void setCoeff(double, double, double, double);
};

class Sphere: public Object {
public:
    double radius;

    Sphere(): Object() { }
    Sphere(double x, double y, double z, double radius,
           double r, double g, double b,
           int shine, double ambient, double diffuse,
           double specular, double recursive);
    Sphere(vector3d center, double radius,
           double r, double g, double b,
           int shine, double ambient, double diffuse,
           double specular, double recursive);
};

class Triangle: public Object {
public:
    vector3d p1, p2, p3;

    Triangle(): Object() { }
    Triangle(double x1, double y1, double z1,
             double x2, double y2, double z2,
             double x3, double y3, double z3,
             double r, double g, double b,
             int shine, double ambient, double diffuse,
             double specular, double recursive);
};


#endif // _OBJECTS_H_
