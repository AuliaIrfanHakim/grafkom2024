#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <cmath>

int SCREEN_WIDTH = 400;
int SCREEN_HEIGHT = 400;

float angle = 0.0f;
float posX = 0.0f, rotX = 0.0f;
float posY = 0.0f, rotY = 0.0f;
float posZ = 5.0f, rotZ = -1.0f;

float objectAngleX = 0.0f;
float objectAngleY = 0.0f;
float objectAngleZ = 0.0f;

struct Vec3 {
    float X, Y, Z;
    Vec3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    Vec3() {}
    ~Vec3() {}
};

void lineDDAX(Vec3 point1, Vec3 point2) {
    int dY = point2.Y - point1.Y;
    int dX = point2.X - point1.X;
    float m = (float)dY / dX;

    glBegin(GL_POINTS);
    float pX = point1.X, pY = point1.Y, pZ = point1.Z;

    for (int i = point1.X; i < point2.X; ++i) {
        pX += 1;
        pY += m;
        glVertex3f(pX, pY, pZ);
    }
    glEnd();
}

void lineDDAY(Vec3 point1, Vec3 point2) {
    int dY = point2.Y - point1.Y;
    int dX = point2.X - point1.X;
    float m = (float)dX / dY;

    glBegin(GL_POINTS);
    float pX = point1.X, pY = point1.Y, pZ = point1.Z;

    for (int i = point1.Y; i < point2.Y; ++i) {
        pX += m;
        pY += 1;
        glVertex3f(pX, pY, pZ);
    }
    glEnd();
}

void lineDDA(Vec3 point1, Vec3 point2) {
    int dY = point2.Y - point1.Y;
    int dX = point2.X - point1.X;

    if (abs(dY) < abs(dX)) {
        if (point1.X < point2.X) 
            lineDDAX(point1, point2);
        else 
            lineDDAX(point2, point1);
    } else {
        if (point1.Y < point2.Y) 
            lineDDAY(point1, point2);
        else 
            lineDDAY(point2, point1);
    }
}

void lineBresenham(Vec3 p1, Vec3 p2) {
    int x1 = p1.X, y1 = p1.Y, x2 = p2.X, y2 = p2.Y;
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    glBegin(GL_POINTS);
    while (true) {
        glVertex2i(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
    glEnd();
}

void drawObject() {
    glPushMatrix();
    glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);
    glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);

    // Diamond menggunakan algoritma Bresenham
    glColor3f(1.0f, 1.0f, 0.0f); // Warna kuning
    Vec3 d1(0.0f, 50.0f, 0.0f);
    Vec3 d2(50.0f, 0.0f, 0.0f);
    Vec3 d3(0.0f, -50.0f, 0.0f);
    Vec3 d4(-50.0f, 0.0f, 0.0f);
    lineBresenham(d1, d2);
    lineBresenham(d2, d3);
    lineBresenham(d3, d4);
    lineBresenham(d4, d1);

    // Segitiga menggunakan algoritma DDA
    glColor3f(0.0f, 0.0f, 1.0f); // Warna biru
    Vec3 t1(-50.0f, -50.0f, 0.0f);
    Vec3 t2(50.0f, -50.0f, 0.0f);
    Vec3 t3(0.0f, 50.0f, 0.0f);
    lineDDA(t1, t2);
    lineDDA(t2, t3);
    lineDDA(t3, t1);

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);
    drawObject();
    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-SCREEN_WIDTH/2, SCREEN_WIDTH/2, -SCREEN_HEIGHT/2, SCREEN_HEIGHT/2, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2, w/2, -h/2, h/2, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC untuk keluar program
}

void specialKeyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        objectAngleY -= 5.0f;
        break;
    case GLUT_KEY_RIGHT:
        objectAngleY += 5.0f;
        break;
    case GLUT_KEY_UP:
        objectAngleX -= 5.0f;
        break;
    case GLUT_KEY_DOWN:
        objectAngleX += 5.0f;
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Diamond dan Segitiga dengan DDA dan Bresenham");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);

    glutMainLoop();
    return 0;
}

