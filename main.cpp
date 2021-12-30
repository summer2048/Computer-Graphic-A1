#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
# include <iostream>
# include <stdlib.h>
# include <vector>
# include <math.h>
# include "mathLib2D.h"


using namespace std;

const int DEFAULT_WIN_SIZE_X = 800;
const int DEFAULT_WIN_SIZE_Y = 600;

const float FOLLOW = 1.0;
const float AWAY = -1.0;
const int DEFAULT_POINT_SIZE = 30;
const float DEFAULT_SPEED = 1.0f;
float speed_constant = 80.0f;
int rangeRadius = 200;
int mouse_button = GLUT_MIDDLE_BUTTON;
bool pause = false;
bool change_color = false;



// Window size.
int winSizeX = 800;
int winSizeY = 600;
// Store Mouse location.
Point2D mouseLocation(0.0f , 0.0f);
//
Particle * part = new Particle[1000];
int pointNum = 0;
// Speed

void term_output1(){
    std::cout << "--------------------------------------HELP--------------------------------------\n" 
        << "Click right/left mouse button to attract/kick away particles. \n"
        << "Press 'a'|'A' to creat a particle at mouse position. \n"
        << "Press 'd'|'D' to delete closest particle. \n"
        << "Press 'r'|'R' to clear screen.\n"
        << "Press 'p'|'P' to pause/ unpause. \n"
        << "Press '+'/'-' to increse/ decrease mouse effect range. \n"
        << "Press up/ down to speed up/ slow down default mouse effect level. \n"
        << "Press 'q'|'Q'|'esc' to exit. \n";

    std::cout << "-----------------------------------END_HELP-------------------------------------" << std::endl;
}


void quit(){
    delete [] part;
    exit(0);
}


float randFloat(){
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void initPoint(float mX, float mY){
    if (pointNum == 1000){
        std::cout << "Maximal particle number reached, particle create unsuccessfully. \n Press 'd' to delete some or press 'r' to clear all. \n";
        return;
    }
    Particle temp_p(mX, mY);
    part[pointNum] = temp_p;
    pointNum++;
}

void initPoint(){
    initPoint(randFloat()*float(DEFAULT_WIN_SIZE_X), randFloat()*float(DEFAULT_WIN_SIZE_Y));
    
}

void initSomePoints(){
    for (int i = 0; i < 20; i++){
        initPoint();
    }
}

// Particle Reaction to mouse click
void followOrAway(float j){
    for (int i = 0; i < pointNum; i++){
        if (mouseLocation.distanceTo(part[i].position.point) < rangeRadius){
            Vec2D v1 = Vec2D::createVector(part[i].position.point, mouseLocation);
            float adjustedLength = 400.0f/(v1.length()+ speed_constant);
            //std::cout << to_string(adjustedLength) << std::endl;
            v1 = part[i].direction.vec.addVec(v1.normalize().multiply(adjustedLength).multiply(j));
            part[i].speed = v1.length();
            part[i].direction.vec = v1.normalize();
        }
    }
}

// Change particle position (and may change direction) every FPS
void moveAll(){
    Point2D attampt_position;
    for (int i = 0; i < pointNum; i++){
        part[i].color.scumble(part[i].life_duration_left);
        attampt_position = part[i].direction.vec.multiply(part[i].speed).movePoint(part[i].position.point);
        if (attampt_position.mX > DEFAULT_WIN_SIZE_X || attampt_position.mX < 0){
            part[i].direction.vec.mX = -part[i].direction.vec.mX;
        }
        if (attampt_position.mY > DEFAULT_WIN_SIZE_Y || attampt_position.mY < 0 ){
            part[i].direction.vec.mY = -part[i].direction.vec.mY;
        }
        part[i].position.point = part[i].direction.vec.multiply(part[i].speed).movePoint(part[i].position.point);
        if (part[i].speed > DEFAULT_SPEED){
            part[i].speed = max(0.98f*part[i].speed, DEFAULT_SPEED);
        }
    }
}

// Mouse click events.
void mouse(int button, int state, int mX, int mY)
{
    mouseLocation.mX = mX*DEFAULT_WIN_SIZE_X/winSizeX;
    mouseLocation.mY = (winSizeY - mY)*DEFAULT_WIN_SIZE_Y/winSizeY;
    
    if (state == GLUT_DOWN)
    {
        mouse_button = button;
        if (button == GLUT_LEFT_BUTTON){
            followOrAway(FOLLOW);
        }
        else if (button = GLUT_RIGHT_BUTTON)
        {
            followOrAway(AWAY);
        }
    } else {
        mouse_button = GLUT_MIDDLE_BUTTON; // Clear
    }
}

// void animation(){
//     int colorId = rand()%6;
//     for (int i = pointNum; i < min(1000, pointNum + 50); i ++)
//     {
//         part[i] = Particle(mouseLocation.mX, mouseLocation.mY, colorId);
//         part[i].speed = rand()%4+7; // Speed between 7 and 10
//     }
//     pointNum = min(1000, pointNum + 50);
// }

// Delete
void deleteParticle(int i){
    pointNum--;
    part[i] = part[pointNum];
}

// When 'd' is pressed
void deleteClosest(){
    if (pointNum == 0)
    {
        return;
    }
    int j = 0;
    float closestFastDistance = 9999999.0f;
    for (int i = 0; i < pointNum; i++){
        if (part[i].position.point.fastDistanceTo(mouseLocation) < closestFastDistance){
            j = i;
            closestFastDistance = part[i].position.point.fastDistanceTo(mouseLocation);
        }
    }
    deleteParticle(j);
}



// Keyboards events.
void keyboard(unsigned char key, int kX, int kY)
{
    switch (key)
    {
    // Quit
    case 'q':
    case 'Q':
    case 27:
        quit();
        break;
    case 'a':
    case 'A':
        initPoint(mouseLocation.mX, mouseLocation.mY);

        //std::cout << to_string(pointNum) << std::endl;
        break;
    case 'r':
    case 'R':
        pointNum = 0;
        initSomePoints();
        glutPostRedisplay();
        break;
    case 'd':
    case 'D':
        deleteClosest();
        break;
    case 'p':
    case 'P':
        pause = !pause;
        break;
    case '+':
        if (rangeRadius < 300){
            rangeRadius += 30;
            std::cout << "New mouse effect range is: " << to_string(rangeRadius) << std::endl;
        }
        break;
    case '-':
        if (rangeRadius > 100){
            rangeRadius -= 30;
            std::cout << "New mouse effect range is: " << to_string(rangeRadius) << std::endl;
        }
        break;
    default:
        break;
    }
}

void drawpoints(){
    for (int i = 0; i < pointNum; i++){
        glColor3f(part[i].color.r, part[i].color.g, part[i].color.b);
        //
        glPointSize(15 + part[i].size * 1);
        glBegin(GL_POINTS);
        glVertex2f(part[i].position.point.mX, part[i].position.point.mY);
        glEnd();
    }
    
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawpoints();
    
    glutSwapBuffers();
    glFlush();
}

// Mouse drag events.
void motion(int mX, int mY){
    mouseLocation.mX = mX*DEFAULT_WIN_SIZE_X/winSizeX;
    mouseLocation.mY = (winSizeY - mY)*DEFAULT_WIN_SIZE_Y/winSizeY;
}


void FPS (int val){
    if (!pause){
        moveAll();
        if (mouse_button == GLUT_LEFT_BUTTON){
            followOrAway(FOLLOW);
        }
        else if (mouse_button == GLUT_RIGHT_BUTTON)
        {
            followOrAway(AWAY);
        }
        glutPostRedisplay();
        //std::cout << to_string(mouseLocation.mX) << " " << to_string(mouseLocation.mY) <<std::endl;
        for (int i = 0; i < pointNum; i++){
            part[i].life_duration_left--;
            int dist = part[i].position.point.distanceTo(mouseLocation);
            if (dist < 100){
                part[i].life_duration_left = min(1800, part[i].life_duration_left + (100-dist)/5); // renew particles that are close to the cursor
            }
            if (part[i].life_duration_left <= 0){
                deleteParticle(i);
            }
        }
    }
    glutTimerFunc(17, FPS, 0);
  	// Rendering every 17ms
  	// 1 sec = 1000, 60 FPS = 1000/60 =~17
}

void passive(int mX, int mY){
    mouseLocation.mX = mX*DEFAULT_WIN_SIZE_X/winSizeX;
    mouseLocation.mY = (winSizeY - mY)*DEFAULT_WIN_SIZE_Y/winSizeY;
}


void special(int key, int x, int y){
    switch (key)
    {
    case GLUT_KEY_UP:
        if (speed_constant >= 20.0f){
            speed_constant /= 2.0f;
        std::cout << "New mouse effect level is: " << to_string(log2(1280.0f/speed_constant)+1.0f) << std::endl;
        }
        break;
    case GLUT_KEY_DOWN:
        if (speed_constant <= 640.0f){
            speed_constant *= 2.0f;
        std::cout << "New mouse effect level is: " << to_string(log2(1280.0f/speed_constant)+1.0f) << std::endl;
        }
        break;
    default:
        break;
    }
} 

void reshape(int w, int h){
    winSizeX = w;
    winSizeY = h;
    std::cout << "New window size: " << to_string(winSizeX) << ", " << to_string(winSizeY) << std::endl;
    
    //gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    glMatrixMode(GL_MODELVIEW); 
    glViewport(0, 0, w, h); 
}


int main(int argc, char **argv)
{
    term_output1();
    glutInit(&argc, argv);
    glutInitWindowSize(winSizeX, winSizeY);
    glutCreateWindow("PARTICAL FLOCKING SYSTEM");
	glutInitDisplayMode(GLUT_RGB | GL_DOUBLE);
	
    gluOrtho2D(0, winSizeX, 0, winSizeY);
	glClearColor(0.0,0.0,0.0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    initSomePoints();

    // Events and display.
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(17, FPS, 0);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passive);
    glutSpecialFunc(special);
    glutReshapeFunc(reshape);
    glutMainLoop();
}
 
