#include "object.h"
#include <iostream>
#include <cfloat>

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

    vec3 ip = r.src + r.dir*t;
    vec3 norm = get_normal(ip);

    if (norm.dot(r.dir) > 0) {
        norm = norm * -1.0;
    }

    Color col = getColorAt(ip) * coeff[0];
    col.clip();

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

        col += light.color * coeff[1] * std::max(lambert, 0.0) * getColorAt(ip);
        col += light.color * coeff[2] * std::max(pow(abs(phong), shine), 0.0);
        col.clip();
        // std::cout << col << "\n";
    }

    if (lvl >= recursion_depth) return col;

    vec3 _rdir = r.dir - norm * (2 * r.dir.dot(norm));
    _rdir.normalize();
    Ray _r(ip + _rdir * 1, _rdir);

    Object *nearest = nullptr;
    double t_min = DBL_MAX;
    for (auto &obj : objects) {
        double t = obj->intersect_param(_r);
        // std::cout << t << "\n";
        if (t > 0 && t < t_min) {
            nearest = obj;
            t_min = t;
        }
    }

    if (nearest != nullptr) {
        col += nearest->intersect(_r, lvl+1) * coeff[3];
        col.clip();
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

void Sphere::draw() const
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

// https://stackoverflow.com/a/43495719/11135136
void Triangle::draw() const
{

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

    if (abs(_a) < 1e-5) return -1;

    vec3 _d = r.src - a;
    double _f = 1/_a;
    double _u = _f * _d.dot(_h);

    if (_u < 0.0 || _u > 1.0) return -1;

    vec3 _q = _d.cross(side1);
    double _v = _f * _q.dot(r.dir);

    if (_v < 0.0 || _u + _v > 1.0) return -1;

    t = _f * side2.dot(_q);

    if (t <= 1e-5) {
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
    Object(1,0,0, 5, 0.4,0.2,0.2,0.2),
    len(tile_width), size(floor_width),
    ref(vec3(-floor_width/2, -floor_width/2, 0)) { }


// https://community.khronos.org/t/draw-a-checker-floor/54183/4
void Floor::draw() const
{

    glBegin(GL_QUADS); {
        for (int i = -(size/len)/2; i < (size/len)/2; i++) {
            for (int j = -(size/len)/2; j < (size/len)/2; j++) {
                if ((i + j) % 2 == 0) glColor3f(1, 1, 1);
                else glColor3f(0, 0, 0);

                glVertex3f(i*len,       j*len,      0);
                glVertex3f((i+1)*len,   j*len,      0);
                glVertex3f((i+1)*len,   (j+1)*len,  0);
                glVertex3f(i*len,       (j+1)*len,  0);
            }
        }
    } glEnd();
}

double Floor::intersect_param(Ray r)
{
    vec3 center(0,0,0);
    vec3 normal(0,0,1);

    double denom = normal.dot(r.dir);

    double t = -1;
    if(abs(denom) > 1e-5) {
        t = ((center - r.src).dot(normal)) / denom;
        vec3 ip = r.src + r.dir*t;
        if (abs(ip.x) > size/2 || abs(ip.y) > size/2)
            t = -1;
    }
    return t;
}
// Color Floor::intersect(Ray r, int lvl) { }

vec3 Floor::get_normal(vec3 ip)
{
    return vec3(0, 0, 1);
}

Color Floor::getColorAt(vec3 &p)
{
    int dx = (abs(p.x) / len) + (p.x < 0);
    int dy = (abs(p.y) / len) + (p.y < 0);

    if (dx%2 == dy%2) return Color(1,1,1);
    return Color(0,0,0);
}



/****************************************************************************
 ******************************* Class General ********************************
 ****************************************************************************/


General::General(double a, double b, double c, double d, double e,
                 double f, double g, double h, double i, double j,
                 double x, double y, double z,
                 double length, double width, double height,
                 double R, double G, double B,
                 int shine, double ambient, double diffuse,
                 double specular, double recursive)
    :
    Object(R, G, B, shine, ambient, diffuse, specular, recursive),
    center(vec3(x,y,z)), length(length), width(width), height(height),
    a(a), b(b), c(c), d(d), e(e), f(f), g(g), h(h), i(i), j(j) { }


void General::draw() const { }

// http://skuld.bmsc.washington.edu/people/merritt/graphics/quadrics.html
double General::intersect_param(Ray r)
{
    double xo = r.src.x;
    double yo = r.src.y;
    double zo = r.src.z;

    double xd = r.dir.x;
    double yd = r.dir.y;
    double zd = r.dir.z;

    // Aq = Axd2 + Byd2 + Czd2 + Dxdyd + Exdzd + Fydzd
    // Bq = 2*Axoxd + 2*Byoyd + 2*Czozd
    //      + D(xoyd + yoxd) + E(xozd + zoxd) + F(yozd + ydzo)
    //      + Gxd + Hyd + Izd
    // Cq = Axo2 + Byo2 + Czo2
    //      + Dxoyo + Exozo + Fyozo
    //      + Gxo + Hyo + Izo + J

    double aq = a*xd*xd + b*yd*yd + c*zd*zd + d*xd*yd + e*xd*zd + f*yd*zd;
    double bq = 2*a*xo*xd + 2*b*yo*yd + 2*c*zo*zd
        + d*(xo*yd+yo*xd) + e*(xo*zd+zo*xd) + f*(yo*zd+zo*yd)
        + g*xd + h*yd + i*zd;
    double cq = a*xo*xo + b*yo*yo + c*zo*zo
        + d*xo*yo + e*xo*zo + f*yo*zo
        + g*xo + h*yo + i*zo + j;

    // 1. Check Aq = 0 (If Aq = 0 then t = -Cq / Bq)
    // 2. If Aq <> 0, then check the discriminant.
    //    If (Bq2 - 4AqCq ) <0.0 then there is no intersection
    // 3. Compute t0 and if t0 > 0 then done else compute t1
    // 4. Once t is found compute Ri = (xi yi zi)

    if (aq == 0) return -cq/bq;
    double discriminant = bq*bq - 4*aq*cq;
    if (discriminant < 0.0) return -1.0;

    // t0 = (-Bq - (Bq2 - 4AqCq)^0.5) / 2Aq
    // t1 = (-Bq + (Bq2 - 4AqCq)^0.5) / 2Aq
    double t0 = (-bq - sqrt(discriminant)) / (2*aq);
    double t1 = (-bq + sqrt(discriminant)) / (2*aq);

    double t = -1.0;
    if (t0 < 0.0 && t1 < 0.0) return -1.0;
    if (t0 >= 0 && t1 >= 0) {
        vec3 dist0 = r.src + r.dir*t0 - center;
        vec3 dist1 = r.src + r.dir*t1 - center;

        char t0_still_in = 1;
        char t1_still_in = 1;

        if ((length > 0) && t0_still_in && (abs(dist0.x) > length)) t0_still_in = 0;
        if ((length > 0) && t1_still_in && (abs(dist1.x) > length)) t1_still_in = 0;
        if ((width > 0)  && t0_still_in && (abs(dist0.y) > width))  t0_still_in = 0;
        if ((width > 0)  && t1_still_in && (abs(dist1.y) > width))  t1_still_in = 0;
        if ((height > 0) && t0_still_in && (abs(dist0.z) > height)) t0_still_in = 0;
        if ((height > 0) && t1_still_in && (abs(dist1.z) > height)) t1_still_in = 0;

        if (t0_still_in && t1_still_in) return std::min(t0, t1);
        if (t0_still_in && !t1_still_in) return t0;
        if (!t0_still_in && t1_still_in) return t1;
        return -1;
    }
    if (t0 > 0.0) return t0;
    return t1;
}

// Color General::intersect(Ray r, int lvl) { }

vec3 General::get_normal(vec3 ip)
{
    double x = ip.x;
    double y = ip.y;
    double z = ip.z;

    vec3 n(2*a*x+d*y+e*z+g, 2*b*y+d*x+f*z+h, 2*c*z+e*x+f*y+i);
    n.normalize();

    return n;
}


#undef pi
