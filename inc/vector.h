#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>


class vec3
{
public:
    double x,y,z;
    vec3() { }
    vec3(double x, double y, double z) : x(x), y(y), z(z) { }
    vec3(const vec3 &o) : x(o.x), y(o.y), z(o.z) { }

    vec3 cross(const vec3 &o) const;
    double dot(const vec3 &o) const;
    vec3 normalize();
    double magnitude() const;

    vec3 operator+(const vec3 &o) const;
    vec3 operator-(const vec3 &o) const;
    vec3 operator*(double m) const;
    vec3 operator/(double d) const;

    friend std::ostream &operator<<(std::ostream &os, const vec3 &v);
    friend std::istream &operator>>(std::istream &is, vec3 &v);
};


#endif // _VECTOR_H_
