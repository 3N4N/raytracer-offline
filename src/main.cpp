#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glut.h>

#include "object.h"



using namespace std;



int recursion_depth;
int screen_size;
int n_obj;
vector<Object *> objects;



void loadData()
{
    ifstream scenefile;
    scenefile.open("scene.txt");

    scenefile >> recursion_depth >> screen_size;
    scenefile >> n_obj;
    cout << recursion_depth <<" "<< screen_size <<" "<< n_obj <<"\n";

    Object *obj;

    while (n_obj--) {
        string objname;
        scenefile >> objname;

        if (objname == "sphere") {
            double x, y, z, radius;
            double R, G, B;
            double ambient, diffuse, specular, recursive;
            int shine;

            scenefile >> x >> y >> z >> radius;
            scenefile >> R >> G >> B;
            scenefile >> ambient >> diffuse >> specular >> recursive;
            scenefile >> shine;

            obj = new Sphere(x,y,z, radius, R,G,B, shine,
                             ambient,diffuse,specular,recursive);
            objects.push_back(obj);
        }
        else if (objname == "triangle") {
            double x1, y1, z1;
            double x2, y2, z2;
            double x3, y3, z3;
            double R, G, B;
            double ambient, diffuse, specular, recursive;
            int shine;

            scenefile >> x1 >> y1 >> z1;
            scenefile >> x2 >> y2 >> z2;
            scenefile >> x3 >> y3 >> z3;
            scenefile >> R >> G >> B;
            scenefile >> ambient >> diffuse >> specular >> recursive;
            scenefile >> shine;

            obj = new Triangle(x1,y1,z1, x2,y2,z2, x3,y3,z3, R,G,B, shine,
                               ambient,diffuse,specular,recursive);
            objects.push_back(obj);
        }
        else if (objname == "general") {
            double a, b, c, d, e, f, g, h, i, j;
            double x, y, z;
            double length, width, height;
            double R, G, B;
            double ambient, diffuse, specular, recursive;
            int shine;

            scenefile >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j;
            scenefile >> R >> G >> B;
            scenefile >> ambient >> diffuse >> specular >> recursive;
            scenefile >> shine;
        }
    }

    scenefile.close();
}

int main()
{
    loadData();

    return 0;
}
