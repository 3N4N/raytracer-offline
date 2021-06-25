#include "object.h"

#define pi (2*acos(0.0))



/****************************************************************************
 ******************************* Class Object *******************************
 ****************************************************************************/

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



/****************************************************************************
 ******************************* Class Sphere *******************************
 ****************************************************************************/

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

void Sphere::draw()
{
    glPushMatrix();
    glTranslated(ref_point.x, ref_point.y, ref_point.z);

    struct vector3d points[100][100];
    int i, j;
    double h, r;
    int stacks = 20;
    int slices = 20;
    //generate points
    for(i = 0; i <= stacks; i++) {
        h = radius * sin(((double)i / (double)stacks) * (pi/2));
        r = radius * cos(((double)i / (double)stacks) * (pi/2));
        for(j = 0; j <= slices; j++) {
            points[i][j].x = r * cos(((double)j / (double)slices) * 2*pi);
            points[i][j].y = r * sin(((double)j / (double)slices) * 2*pi);
            points[i][j].z = h;
        }
    }
    //draw quads using generated points
    glColor3f(color[0], color[1], color[2]);
    for(i = 0; i < stacks; i++) {
        for(j = 0; j < slices; j++) {
            glBegin(GL_QUADS); {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            } glEnd();
        }
    }

    glPopMatrix();
}



/****************************************************************************
 ***************************** Class Triangle *******************************
 ****************************************************************************/

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

void Triangle::draw()
{
    // https://stackoverflow.com/a/43495719/11135136

    glBegin(GL_TRIANGLES); {
        glColor3f(color[0], color[1], color[2]);
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p3.x, p3.y, p3.z);
    } glEnd();
}




/****************************************************************************
 ******************************* Class Floor ********************************
 ****************************************************************************/

Floor::Floor() : Object(0.0,0.0,0.0, 0,0,0, 0, 0.3,0.3,0.0,0.3) { }

void Floor::draw()
{
    // https://community.khronos.org/t/draw-a-checker-floor/54183/4

    glBegin(GL_QUADS); {
        for (int i = -50; i < 50; i++) {
            for (int j = -50; j < 50; j++) {
                if ((i + j) % 2 == 0) glColor3f(1, 1, 1);
                else glColor3f(0, 0, 0);

                glVertex3f(i*20,        j*20,       0);
                glVertex3f((i+1)*20,    j*20,       0);
                glVertex3f((i+1)*20,    j*20+20,    0);
                glVertex3f(i*20,        j*20+20,    0);
            }
        }
    } glEnd();
}

#undef pi
