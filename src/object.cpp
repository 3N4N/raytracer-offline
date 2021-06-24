#include "object.h"


Object::Object()
{

}

Object::~Object()
{

}

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
