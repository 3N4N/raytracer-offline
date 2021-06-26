#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include <GL/glut.h>
#include <cmath>
#include <vector>
#include "vector.h"
#include "color.h"
#include "light.h"

class Object;

extern int recursion_depth;
extern std::vector<Light> lights;
extern std::vector<Object *> objects;

class Ray {
public:
    vec3 src;
    vec3 dir;

    Ray() { }
    Ray(vec3 src, vec3 dir);
    Ray(double srcX, double srcY, double srcZ,
        double dirX, double dirY, double dirZ);
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
    virtual void draw() const = 0;
    virtual double intersect_param(Ray r) = 0;
    virtual vec3 get_normal(vec3 ip) = 0;
    Color intersect(Ray r, int lvl = 0);
    void setColor(double, double, double);
    void setShine(int);
    void setCoeff(double, double, double, double);
    virtual Color getColorAt(vec3 &p);
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
    void draw() const override;
    double intersect_param(Ray r) override;
    // Color intersect(Ray r, int lvl = 0);
    vec3 get_normal(vec3 ip) override;
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
    void draw() const override;
    double intersect_param(Ray r) override;
    // Color intersect(Ray r, int lvl = 0);
    vec3 get_normal(vec3 ip) override;
};

class Floor: public Object {
public:
    vec3 ref;
    double len;
    double size;

    Floor();
    Floor(int floor_width, int tile_width);
    void draw() const override;
    double intersect_param(Ray r) override;
    // Color intersect(Ray r, int lvl = 0);
    vec3 get_normal(vec3 ip) override;
    Color getColorAt(vec3 &p) override;
};


#endif // _OBJECTS_H_
