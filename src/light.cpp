#include "light.h"
#include <GL/glut.h>

Light::Light(double x, double y, double z,
             double r, double g, double b)
{
    setPosition(x, y, z);
    setColor(r, g, b);
}

void Light::setPosition(double x, double y, double z)
{
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;
}

void Light::setColor(double r, double g, double b)
{
    color.r = r;
    color.g = g;
    color.b = b;
}

void Light::draw() const
{
    glPointSize(5);
    glColor3f(color.r , color.g , color.b);
    glBegin(GL_POINTS);
    glVertex3f(pos.x , pos.y , pos.z);
    glEnd();
}

