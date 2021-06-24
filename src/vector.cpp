#include "vector.h"


double dot_product(vector2d a, vector2d b)
{
    return a.x * b.x + a.y * b.y;
}

vector3d cross_product(vector3d a, vector3d b)
{
    vector3d prod;

    prod.x = a.y * b.z - b.y * a.z;
    prod.y = a.z * b.x - b.z * a.x;
    prod.z = a.x * b.y - b.x * a.y;

    return prod;
}

double dot_product(vector3d a, vector3d b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
