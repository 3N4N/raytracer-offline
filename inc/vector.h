#ifndef _VECTOR_H_
#define _VECTOR_H_

struct vector2d
{
    double x,y;
};

struct vector3d
{
    double x,y,z;
};

vector2d cross_product(vector2d a, vector2d b);
double dot_product(vector2d a, vector2d b);

vector3d cross_product(vector3d a, vector3d b);
double dot_product(vector3d a, vector3d b);

#endif // _VECTOR_H_
