#include <windows.h>
#include <GL/glut.h>

float angle = 0.0f;                                        
float posX = 0.0f, rotX =  0.0f;        
float posY = 0.0f, rotY =  0.0f;        
float posZ = 5.0f, rotZ = -1.0f;        

float objectAngleX = 0.0f;                        
float objectAngleY = 0.0f;                        
float objectAngleZ = 0.0f;                        

float objectScaleX = 1.0f;        
float objectScaleY = 1.0f;        
float objectScaleZ = 1.0f;        

float objectPositionX = 0.0f;        
float objectPositionY = 0.0f;        
float objectPositionZ = 0.0f;        

void drawCylinder(float radius, float height, int slices) {
    GLUquadricObj *obj = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricNormals(obj, GLU_SMOOTH);

    glColor3f(0.5f, 0.5f, 1.0f);  // Warna biru untuk tabung

    // Alas tabung
    gluDisk(obj, 0, radius, slices, 1);

    // Sisi tabung
    gluCylinder(obj, radius, radius, height, slices, 1);

    // Tutup tabung
    glTranslatef(0.0f, 0.0f, height);
    gluDisk(obj, 0, radius, slices, 1);

    gluDeleteQuadric(obj);
}


void drawObject() {
    glPushMatrix();

    glTranslatef(objectPositionX, objectPositionY, objectPositionZ);
    glScalef(objectScaleX, objectScaleY, objectScaleZ);
    glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);
    glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);
    
    drawCylinder(0.5f, 1.5f, 50);  // Jari-jari, tinggi, dan jumlah sisi
    
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);
    drawObject();
    glutSwapBuffers();
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);                
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);                                
    glLoadIdentity();                                                
    gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(int key, int x, int y) {
    float fraction = 0.1f;

    switch (key) {
    case GLUT_KEY_LEFT:
        objectAngleY -= 1.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT:                
        objectAngleY += 1.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_UP:                
        objectAngleX -= 1.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN:                
        objectAngleX += 1.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_PAGE_UP:
        posX += rotX * fraction;                
        posZ += rotZ * fraction;
        glutPostRedisplay();
        break;
    case GLUT_KEY_PAGE_DOWN:
        posX -= rotX * fraction;
        posZ -= rotZ * fraction;
        glutPostRedisplay();
        break;
    }
}

void keyboard1(unsigned char key, int x, int y) {
    float fraction = 0.5f;

    switch (key) {
    case 'w':
        objectPositionY += fraction;
        glutPostRedisplay();
        break;
    case 's':
        objectPositionY -= fraction;
        glutPostRedisplay();
        break;
    case 'a':
        objectPositionX -= fraction;
        glutPostRedisplay();
        break;
    case 'd':
        objectPositionX += fraction;
        glutPostRedisplay();
        break;        
    case 'q':
        objectPositionZ += fraction;
        glutPostRedisplay();
        break;
    case 'e':
        objectPositionZ -= fraction;
        glutPostRedisplay();
        break;
    case 't':
        objectScaleY += 0.1f;
        glutPostRedisplay();
        break;
    case 'g':
        objectScaleY -= 0.1f;
        if (objectScaleY < 0.1f) objectScaleY = 0.1f;
        glutPostRedisplay();
        break;
    case 'f':
        objectScaleX += 0.1f;
        glutPostRedisplay();
        break;
    case 'h':
        objectScaleX -= 0.1f;
        if (objectScaleX < 0.1f) objectScaleX = 0.1f;
        glutPostRedisplay();
        break;
    case 'r':
        objectScaleZ += 0.1f;
        glutPostRedisplay();
        break;
    case 'y':
        objectScaleZ -= 0.1f;
        if (objectScaleZ < 0.1f) objectScaleZ = 0.1f;
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
        break;
    }
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(55, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(480, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("AULIA IRFAN HAKIM / 2200018329 - POST TEST 4 GRAFIKA KOMPUTER");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    glutKeyboardFunc(keyboard1);
    glutMainLoop();
    return 0;
}

