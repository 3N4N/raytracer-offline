#include "object.h"
#include <iostream>

#define pi (2*acos(0.0))

/****************************************************************************
 ******************************** Class Ray *********************************
 ****************************************************************************/

Ray::Ray(vec3 src, vec3 dir)
    : src(src), dir(dir) { }

Ray::Ray(double srcX, double srcY, double srcZ,
         double dirX, double dirY, double dirZ)
    :
    src(vec3(srcX,srcY,srcZ)),
    dir(vec3(dirX,dirY,dirZ)) { }

void Ray::setT(double t)
{
    this->t = t;
}

void Ray::setColor(Color c)
{
    color = c;
}

/****************************************************************************
 ******************************* Class Object *******************************
 ****************************************************************************/

Object::Object() { }

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
    color.r = r;
    color.g = g;
    color.b = b;
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

Color Object::getColorAt(vec3 &p)
{
    return color;
}

Color Object::intersect(Ray r, int lvl)
{
    // std::cout << r.src << r.dir << "\n";
    double t = intersect_param(r);

    Color col = color * coeff[0];

    vec3 ip = r.src + r.dir*t;
    vec3 norm = get_normal(ip);

    for (Light &light : lights) {
        Ray lightray(light.pos, (ip - light.pos).normalize());
        // std::cout << lightray.src << lightray.dir << "\n";

        double _t = (ip.x - lightray.src.x) / lightray.dir.x;
        // std::cout << _t << "\n";

        unsigned char hidden = 0;
        for (auto &obj : objects) {
            double t = obj->intersect_param(lightray);
            // std::cout << t << "\n";
            if (t > 0 && floor(t) < floor(_t)) {
                hidden = 1;
                break;
            }
        }

        if (hidden) continue;

        double lambert = norm.dot(lightray.dir);
        vec3 R = lightray.dir - (norm * (2 * lambert));
        double phong = R.dot(r.dir);
        // std::cout << lambert << "\n";
        // std::cout << R << "\n";
        // std::cout << phong << "\n";

        col += light.color * col * coeff[1] * lambert;
        col += light.color * col * coeff[2] * pow(abs(phong), shine);
        // std::cout << col << "\n";
    }

    return col;
}



/****************************************************************************
 ******************************* Class Sphere *******************************
 ****************************************************************************/

Sphere::Sphere(double x, double y, double z, double radius,
               double r, double g, double b,
               int shine, double ambient, double diffuse,
               double specular, double recursive)
    :
    Object(r, g, b, shine, ambient, diffuse, specular, recursive),
    center(vec3(x,y,z)), radius(radius) { }

Sphere::Sphere(vec3 center, double radius,
               double r, double g, double b,
               int shine, double ambient, double diffuse,
               double specular, double recursive)
    : Sphere(center.x, center.y, center.z, radius, r, g, b,
             shine, ambient, diffuse, specular, recursive) { }

void Sphere::draw()
{
    glPushMatrix();
    glTranslated(center.x, center.y, center.z);

    struct vec3 points[100][100];
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
    glColor3f(color.r, color.g, color.b);
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


// https://viclw17.github.io/2018/07/16/raytracing-ray-sphere-intersection/
double Sphere::intersect_param(Ray r)
{
    // std::cout << r.src << r.dir << "\n";
    vec3 oc = r.src - center;
    double a = r.dir.dot(r.dir);
    double b = 2.0 * oc.dot(r.dir);
    double c = oc.dot(oc) - radius*radius;
    double discriminant = b*b - 4*a*c;
    // std::cout << a <<","<< b <<","<< c << "\n";
    // if (discriminant >= 0.0)
        // std::cout << discriminant << "\n";

    if (discriminant < 0) {
        return -1.0;
    }

    double numerator = -b - sqrt(discriminant);
    // std::cout << "[DBG]" << numerator << "\n";

    if (numerator > 0.0) {
        return numerator / (2.0 * a);
    } else {
        numerator = (-b + sqrt(discriminant)) / (2.0*a);
        if (numerator > 0.0) {
            return numerator / (2.0 * a);
        } else {
            return -1;
        }
    }
}


vec3 Sphere::get_normal(vec3 ip)
{
    vec3 ret;
    ret = ip - center;
    ret.normalize();
    return ret;
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
    :
    Object(r, g, b, shine, ambient, diffuse, specular, recursive),
    a(vec3(x1,y1,z1)), b(vec3(x2,y2,z2)), c(vec3(x3,y3,z3)) { }

void Triangle::draw()
{
    // https://stackoverflow.com/a/43495719/11135136

    glBegin(GL_TRIANGLES); {
        glColor3f(color.r, color.g, color.b);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
    } glEnd();
}

// https://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/
double Triangle::intersect_param(Ray r)
{
    double t;

    vec3 side1 = b - a;
    vec3 side2 = c - a;

    vec3 _h = r.dir.cross(side2);
    double _a = _h.dot(side1);

    if (abs(_a) < 0.00001) return -1;

    vec3 _d = r.src - a;
    double _f = 1/_a;
    double _u = _f * _d.dot(_h);

    if (_u < 0.0 || _u > 1.0) return -1;

    vec3 _q = _d.cross(side1);
    double _v = _f * _q.dot(r.dir);

    if (_v < 0.0 || _u + _v > 1.0) return -1;

    t = _f * side2.dot(_q);

    if (t <= 0.00001) {
        return -1;
    }

    return t;
}

// Color Triangle::intersect(Ray r, int lvl) { }

vec3 Triangle::get_normal(vec3 ip)
{
    vec3 side1 = b - a;
    vec3 side2 = c - a;
    vec3 normal = side1.cross(side2);
    normal.normalize();
    return normal;
}



/****************************************************************************
 ******************************* Class Floor ********************************
 ****************************************************************************/

Floor::Floor() : Object(0,0,0, 0, 0.3,0.3,0.0,0.3) { }

Floor::Floor(int floor_width, int tile_width)
    :
    Object(0,0,0, 0, 0.3,0.3,0.0,0.3),
    floor_width(floor_width), tile_width(tile_width) { }

void Floor::draw()
{
    // https://community.khronos.org/t/draw-a-checker-floor/54183/4

    glBegin(GL_QUADS); {
        for (int i = -floor_width/2; i < floor_width/2; i++) {
            for (int j = -floor_width/2; j < floor_width/2; j++) {
                if ((i + j) % 2 == 0) glColor3f(1, 1, 1);
                else glColor3f(0, 0, 0);

                glVertex3f(i*tile_width,        j*tile_width,       0);
                glVertex3f((i+1)*tile_width,    j*tile_width,       0);
                glVertex3f((i+1)*tile_width,    (j+1)*tile_width,   0);
                glVertex3f(i*tile_width,        (j+1)*tile_width,   0);
            }
        }
    } glEnd();
}

double Floor::intersect_param(Ray r) { }
// Color Floor::intersect(Ray r, int lvl) { }

vec3 Floor::get_normal(vec3 ip)
{
    return vec3(0, 0, 1);
}


#undef pi
