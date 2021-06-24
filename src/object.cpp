#include "object.h"


Object::Object() { }

Object::Object(double x, double y, double z,
               double r, double g, double b,
               int shine, double ambient, double diffuse,
               double specular, double recursive)
{
    ref_point.x = x;
    ref_point.y = y;
    ref_point.z = z;

    setColor(r, g, b);
    setShine(shine);
    setCoeff(ambient, diffuse, specular, recursive);
}

Object::Object(vector3d ref, double r, double g, double b,
               int shine, double ambient, double diffuse,
               double specular, double recursive)
    : Object(ref.x, ref.y, ref.z, r, g, b,
             shine, ambient, diffuse, specular, recursive) { }

Object::Object(double r, double g, double b,
               int shine, double ambient, double diffuse,
               double specular, double recursive)
{
    setColor(r, g, b);
    setShine(shine);
    setCoeff(ambient, diffuse, specular, recursive);
}

Object::~Object() { }

void Object::setColor(double r, double g, double b)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

void Object::setShine(int shine)
{
    this->shine = shine;
}

void Object::setCoeff(double ambient, double diffuse,
                      double specular, double reflection)
{
    coeff[0] = ambient;
    coeff[1] = diffuse;
    coeff[2] = specular;
    coeff[3] = reflection;
}



Sphere::Sphere(double x, double y, double z, double radius,
               double r, double g, double b,
               int shine, double ambient, double diffuse,
               double specular, double recursive)
    :
    Object(x, y, z, r, g, b, shine, ambient, diffuse, specular, recursive),
    radius(radius) { }

Sphere::Sphere(vector3d center, double radius,
               double r, double g, double b,
               int shine, double ambient, double diffuse,
               double specular, double recursive)
    :
    Sphere(center.x, center.y, center.z, radius, r, g, b,
           shine, ambient, diffuse, specular, recursive) { }



Triangle::Triangle(double x1, double y1, double z1,
         double x2, double y2, double z2,
         double x3, double y3, double z3,
         double r, double g, double b,
         int shine, double ambient, double diffuse,
         double specular, double recursive)
    : Object(r, g, b, shine, ambient, diffuse, specular, recursive)
{
    p1.x = x1; p1.y = y1; p1.z = z1;
    p2.x = x2; p2.y = y2; p2.z = z2;
    p3.x = x3; p3.y = y3; p3.z = z3;
}
