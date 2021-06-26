#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include <GL/glut.h>
#include <cmath>
#include <vector>
#include "vector.h"
#include "color.h"
#include "light.h"

class Object;

extern std::vector<Light> lights;
extern std::vector<Object *> objects;

class Ray {
public:
    vec3 src;
    vec3 dir;
    double t;
    Color color;

    Ray() { }
    Ray(vec3 src, vec3 dir);
    Ray(double srcX, double srcY, double srcZ,
        double dirX, double dirY, double dirZ);
    void setT(double t);
    void setColor(Color c);
};


class Object {
public:
    Color color;
    double coeff[4]; // reflection coefficients
    int shine; // exponent term of specular component

    Object();
    Object(double r, double g, double b, int shine,
           double ambient, double diffuse,
           double specular, double recursive);
    ~Object();
    virtual void draw() = 0;
    virtual double intersect_param(Ray r) = 0;
    Color illuminate(const Ray r, vec3 ip, double t, int lvl);
    virtual vec3 get_normal(vec3 ip) = 0;
    Color intersect(Ray r, int lvl = 0);
    void setColor(double, double, double);
    void setShine(int);
    void setCoeff(double, double, double, double);
    Color getColorAt(vec3 &p);
};

class Sphere: public Object {
public:
    vec3 center;
    double radius;

    Sphere(): Object() { }
    Sphere(double x, double y, double z, double radius,
           double r, double g, double b,
           int shine, double ambient, double diffuse,
           double specular, double recursive);
    Sphere(vec3 center, double radius,
           double r, double g, double b,
           int shine, double ambient, double diffuse,
           double specular, double recursive);
    void draw();
    double intersect_param(Ray r);
    // Color intersect(Ray r, int lvl = 0);
    vec3 get_normal(vec3 ip);
};

class Triangle: public Object {
public:
    vec3 a, b, c;

    Triangle(): Object() { }
    Triangle(double x1, double y1, double z1,
             double x2, double y2, double z2,
             double x3, double y3, double z3,
             double r, double g, double b,
             int shine, double ambient, double diffuse,
             double specular, double recursive);
    void draw();
    double intersect_param(Ray r);
    // Color intersect(Ray r, int lvl = 0);
    vec3 get_normal(vec3 ip);
};

class Floor: public Object {
public:
    int floor_width, tile_width;

    Floor();
    Floor(int floor_width, int tile_width);
    void draw();
    double intersect_param(Ray r);
    // Color intersect(Ray r, int lvl = 0);
    vec3 get_normal(vec3 ip);
};


#endif // _OBJECTS_H_
