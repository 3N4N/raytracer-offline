#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include <GL/glut.h>
#include <cmath>
#include "vector.h"


class Object {
public:
    double color[3];
    double coeff[4]; // reflection coefficients
    int shine; // exponent term of specular component

    Object();
    Object(double r, double g, double b, int shine,
           double ambient, double diffuse,
           double specular, double recursive);
    ~Object();
    virtual void draw() = 0;
    void setColor(double, double, double);
    void setShine(int);
    void setCoeff(double, double, double, double);
};

class Sphere: public Object {
public:
    vector3d center;
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
    void draw();
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
    void draw();
};

class Floor: public Object {
public:
    int floor_width, tile_width;

    Floor();
    Floor(int floor_width, int tile_width);
    void draw();
};


#endif // _OBJECTS_H_
