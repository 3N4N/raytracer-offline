#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glut.h>

#include "object.h"
#include "light.h"

#define pi (2*acos(0.0))
#define ROTSPEED 0.8
#define MOVSPEED 3


using namespace std;



int recursion_depth;
int screen_size;
int n_obj;

vector3d cam_pos;
vector3d cam_u, cam_l, cam_r;

vector<Object *> objects;
vector<Light> lights;



void loadData()
{
    ifstream scenefile;
    scenefile.open("scene.txt");

    scenefile >> recursion_depth >> screen_size;
    scenefile >> n_obj;

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

    int n_light;
    scenefile >> n_light;
    while (n_light--) {
        double x, y, z;
        double r, g, b;
        scenefile >> x >> y >> z;
        scenefile >> r >> g >> b;
        lights.push_back(Light(x,y,z, r,g,b));
    }

    scenefile.close();
}

void keyboardListener(unsigned char key, int x, int y)
{
    double radangle = ROTSPEED * pi / 180;
    vector3d _cam_l, _cam_r, _cam_u;
    vector3d _gun_l, _gun_r, _gun_u;
    _cam_l.x = cam_l.x; _cam_l.y = cam_l.y; _cam_l.z = cam_l.z;
    _cam_r.x = cam_r.x; _cam_r.y = cam_r.y; _cam_r.z = cam_r.z;
    _cam_u.x = cam_u.x; _cam_u.y = cam_u.y; _cam_u.z = cam_u.z;

    switch(key){

        case '1':
            cam_l.x = _cam_l.x * cos(radangle) - _cam_r.x * sin(radangle);
            cam_l.y = _cam_l.y * cos(radangle) - _cam_r.y * sin(radangle);
            cam_l.z = _cam_l.z * cos(radangle) - _cam_r.z * sin(radangle);
            cam_r = cross_product(cam_l, cam_u);
            break;
        case '2':
            cam_l.x = _cam_l.x * cos(-1 * radangle) - _cam_r.x * sin(-1 * radangle);
            cam_l.y = _cam_l.y * cos(-1 * radangle) - _cam_r.y * sin(-1 * radangle);
            cam_l.z = _cam_l.z * cos(-1 * radangle) - _cam_r.z * sin(-1 * radangle);
            cam_r = cross_product(cam_l, cam_u);
            break;

        case '3':
            cam_l.x = _cam_l.x * cos(radangle) + _cam_u.x * sin(radangle);
            cam_l.y = _cam_l.y * cos(radangle) + _cam_u.y * sin(radangle);
            cam_l.z = _cam_l.z * cos(radangle) + _cam_u.z * sin(radangle);
            cam_u = cross_product(cam_r, cam_l);
            break;
        case '4':
            cam_l.x = _cam_l.x * cos(-1 * radangle) + _cam_u.x * sin(-1 * radangle);
            cam_l.y = _cam_l.y * cos(-1 * radangle) + _cam_u.y * sin(-1 * radangle);
            cam_l.z = _cam_l.z * cos(-1 * radangle) + _cam_u.z * sin(-1 * radangle);
            cam_u = cross_product(cam_r, cam_l);
            break;

        case '5':
            cam_u.x = _cam_u.x * cos(-1 * radangle) + _cam_r.x * sin(-1 * radangle);
            cam_u.y = _cam_u.y * cos(-1 * radangle) + _cam_r.y * sin(-1 * radangle);
            cam_u.z = _cam_u.z * cos(-1 * radangle) + _cam_r.z * sin(-1 * radangle);
            cam_r = cross_product(cam_l, cam_u);
            break;
        case '6':
            cam_u.x = _cam_u.x * cos(radangle) + _cam_r.x * sin(radangle);
            cam_u.y = _cam_u.y * cos(radangle) + _cam_r.y * sin(radangle);
            cam_u.z = _cam_u.z * cos(radangle) + _cam_r.z * sin(radangle);
            cam_r = cross_product(cam_l, cam_u);
            break;

        case '0':
            break;

        default:
            break;
    }
}

void specialKeyListener(int key, int x, int y)
{
    switch(key){
        case GLUT_KEY_UP:       // up arrow key
            // cameraHeight += 3.0;
            cam_pos.x += cam_l.x * MOVSPEED;
            cam_pos.y += cam_l.y * MOVSPEED;
            cam_pos.z += cam_l.z * MOVSPEED;
            break;
        case GLUT_KEY_DOWN:     //down arrow key
            // cameraHeight -= 3.0;
            cam_pos.x -= cam_l.x * MOVSPEED;
            cam_pos.y -= cam_l.y * MOVSPEED;
            cam_pos.z -= cam_l.z * MOVSPEED;
            break;

        case GLUT_KEY_RIGHT:
            // cameraAngle += 0.03;
            cam_pos.x += cam_r.x * MOVSPEED;
            cam_pos.y += cam_r.y * MOVSPEED;
            cam_pos.z += cam_r.z * MOVSPEED;
            break;
        case GLUT_KEY_LEFT:
            // cameraAngle -= 0.03;
            cam_pos.x -= cam_r.x * MOVSPEED;
            cam_pos.y -= cam_r.y * MOVSPEED;
            cam_pos.z -= cam_r.z * MOVSPEED;
            break;

        case GLUT_KEY_PAGE_UP:
            cam_pos.x += cam_u.x * MOVSPEED;
            cam_pos.y += cam_u.y * MOVSPEED;
            cam_pos.z += cam_u.z * MOVSPEED;
            break;
        case GLUT_KEY_PAGE_DOWN:
            cam_pos.x -= cam_u.x * MOVSPEED;
            cam_pos.y -= cam_u.y * MOVSPEED;
            cam_pos.z -= cam_u.z * MOVSPEED;
            break;

        default:
            break;
    }
}

void mouseListener(int button, int state, int x, int y)
{
    //x, y is the x-y of the screen (2D)
    switch(button){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN) {
            }
            break;

        case GLUT_RIGHT_BUTTON:
            break;

        case GLUT_MIDDLE_BUTTON:
            break;

        default:
            break;
    }
}


void display()
{
    // clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);  // color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    // load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    // initialize the matrix
    glLoadIdentity();

    gluLookAt(cam_pos.x, cam_pos.y, cam_pos.z,
              cam_pos.x+cam_l.x, cam_pos.y+cam_l.y, cam_pos.z+cam_l.z,
              cam_u.x, cam_u.y, cam_u.z);
    // gluLookAt(0,0,200,  0,0,0,  0,1,0);

    // again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);

    /****************************
    / Add your objects from here
    ****************************/

    for (const auto &object : objects) object->draw();


    // ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}

void animate()
{
    // codes for any changes in Models, Camera
    glutPostRedisplay();
}


void init()
{

    cam_pos.x = 100;
    cam_pos.y = 100;
    cam_pos.z = 0;

    cam_u.x = 0;
    cam_u.y = 0;
    cam_u.z = 1;

    cam_r.x = -1.0/sqrt(2);
    cam_r.y = 1.0/sqrt(2);
    cam_r.z = 0;

    cam_l.x = -1.0/sqrt(2);
    cam_l.y = -1.0/sqrt(2);
    cam_l.z = 0;

    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80,  1,  1,  1000.0);
}


int main(int argc, char **argv)
{
    objects.push_back(new Floor(1000, 20));
    loadData();

    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow("Ray Tracing");

    init();

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutIdleFunc(animate);

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();


    return 0;
}
