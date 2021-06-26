#include "vector.h"
#include <cmath>


vec3 vec3::cross(const vec3 &o) const
{
    vec3 prod;

    prod.x = y * o.z - o.y * z;
    prod.y = z * o.x - o.z * x;
    prod.z = x * o.y - o.x * y;

    return prod;
}

double vec3::dot(const vec3 &o) const
{
    return x * o.x + y * o.y + z * o.z;
}

vec3 vec3::normalize()
{
    double r = sqrt(x * x + y * y + z * z);
    x /= r;
    y /= r;
    z /= r;
    return *this;
}

double vec3::magnitude() const
{
    return sqrt(x * x + y * y + z * z);
}

vec3 vec3::operator+(const vec3 &o) const
{
    return vec3(x+o.x, y+o.y, z+o.z);
}

vec3 vec3::operator-(const vec3 &o) const
{
    return vec3(x-o.x, y-o.y, z-o.z);
}


vec3 vec3::operator*(double m) const
{
    return vec3(x*m, y*m, z*m);
}

vec3 vec3::operator/(double d) const
{
    return vec3(x/d, y/d, z/d);
}

std::ostream &operator<<(std::ostream &os, const vec3 &v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

std::istream &operator>>(std::istream &is, vec3 &v)
{
    is >> v.x >> v.y >> v.z;
    return is;
}
