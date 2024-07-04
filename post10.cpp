// deklarasikan semua header disini
#include <windows.h>
#include <math.h>
#include <GL/glut.h>

// inisialisasi variabel untuk transformasi seperti translasi, rotasi atau scaling
float angle = 0.0f;                 // sudut transformasi kamera
float posX = 0.0f, rotX =  0.0f;    // posisi kamera di sumbu X
float posY = 0.0f, rotY =  0.0f;    // posisi kamera di sumbu Y
float posZ = 5.0f, rotZ = -1.0f;    // posisi kamera di sumbu Z

float objectAngleX = 0.0f;          // sudut tranformasi obyek di sumbu X
float objectAngleY = 0.0f;          // sudut tranformasi obyek di sumbu Y
float objectAngleZ = 0.0f;          // sudut tranformasi obyek di sumbu Z

GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_diffuse[] = { 0.8, 0.6, 0.4, 1.0 };
GLfloat mat_ambient[] = { 0.8, 0.6, 0.4, 1.0 };
GLfloat mat_shininess = 100.0;  /* unused if specular is 0 */

GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };

GLfloat light_position1[] = { 1.5, 1.0, -2.0, 0.0 }; /* directional */
GLfloat light_position2[] = { 1.5, 1.0, 2.0, 0.0 };  /* directional */

int flat = 1;            /* 0 = smooth shading, 1 = flat shading */

// fungsi untuk menggambar obyek torus
// variabel untuk mengontrol detail torus
int numSides = 20;  // jumlah sisi dalam cincin
int numRings = 20;  // jumlah cincin di sekitar torus

// fungsi untuk menggambar obyek torus
void drawObject()
{
    // Menggambar donat luar
    glPushMatrix();
    glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);  // Rotasi donat luar
    glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);
    glutSolidTorus(0.5, 1.5, numSides, numRings);  // Donat luar (radius 0.5, lubang 1.5)

    // Menggambar donat dalam
    glTranslatef(0.0f, 0.0f, -1.0f);  // Posisi donat dalam di dalam donat luar
    glRotatef(objectAngleY * 2, 0.0f, 1.0f, 0.0f);  // Rotasi donat dalam lebih cepat
    glRotatef(objectAngleX * 2, 1.0f, 0.0f, 0.0f);
    glutSolidTorus(0.3, 0.5, numSides, numRings);  // Donat dalam (radius 0.3, lubang 0.5)

    glPopMatrix();
}



// taruh semua obyek yang akan digambar di fungsi display()
void display()
{
    // bersihkan dan reset layar dan buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // posisikan kamera pandang
    // dalam hal ini sumbu Y ada diatas dan posisi kamera pandang di (posX, posY, posZ)
    gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);

    // panggil fungsi untuk menggambar obyek
    drawObject();
    
    // tampilkan obyek ke layar
    // gunakan glFlush() bila memakai single buffer
    // gunakan glutSwapBuffers() bila memakai double buffer
    glutSwapBuffers();
}

// inisialisasikan variabel, pencahayaan, tekstur dan pengaturan kamera pandang di fungsi init()
void init(void)
{
    // inisialisasi warna latar belakang layar dalam hal ini warna putih (1.0, 1.0, 1.0, 0.0)
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);                // mengaktifkan depth buffer
    glMatrixMode(GL_PROJECTION);        
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);  // set proyeksi ke perspektif
    glMatrixMode(GL_MODELVIEW);                
    glLoadIdentity();                        
    // inisialisasi kamera pandang
    gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // inisialisasi pencahayaan
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_LIGHT0, GL_SHININESS, mat_shininess);
    glMaterialf(GL_LIGHT1, GL_SHININESS, mat_shininess);

    glShadeModel(GL_SMOOTH);    /* enable smooth shading */
    glEnable(GL_LIGHTING);    /* enable lighting */
    glEnable(GL_LIGHT0);        /* enable light 0 */
    glEnable(GL_LIGHT1);        /* enable light 0 */
}

// fungsi ini digunakan bila layar akan diresize (default)
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// fungsi untuk mengatur masukan dari keyboard 
// untuk arah kiri, kanan, atas, bawah, PgUp, dan PgDn
void keyboard(int key, int x, int y)
{
    float fraction = 0.1f;

    switch (key) 
    {
    // masukkan perintah disini bila tombol kiri ditekan
    case GLUT_KEY_LEFT:
        // dalam hal ini perintah rotasi obyek ke kiri sebanyak 1 derajat 
        objectAngleY -= 1.0f;
        glutPostRedisplay();    // update obyek
        break;
    // masukkan perintah disini bila tombol kanan ditekan
    case GLUT_KEY_RIGHT:        
        // dalam hal ini perintah rotasi obyek ke kanan sebanyak 1 derajat 
        objectAngleY += 1.0f;
        glutPostRedisplay();    // update obyek
        break;
    // masukkan perintah disini bila tombol atas ditekan
    case GLUT_KEY_UP:        
        // dalam hal ini perintah rotasi obyek ke atas sebanyak 1 derajat 
        objectAngleX -= 1.0f;
        glutPostRedisplay();    // update obyek
        break;
    // masukkan perintah disini bila tombol bawah ditekan
    case GLUT_KEY_DOWN:        
        // dalam hal ini perintah rotasi obyek ke bawah sebanyak 1 derajat 
        objectAngleX += 1.0f;
        glutPostRedisplay();    // update obyek
        break;
        // masukkan perintah disini bila tombol PgUp ditekan
    case GLUT_KEY_PAGE_UP:
        // masukkan perintah disini bila tombol PgUp ditekan
        posX += rotX * fraction;        
        posZ += rotZ * fraction;
        glutPostRedisplay();    // update obyek
        break;
        // masukkan perintah disini bila tombol PgDn ditekan
    case GLUT_KEY_PAGE_DOWN:
        // masukkan perintah disini bila tombol PgDn ditekan
        posX -= rotX * fraction;
        posZ -= rotZ * fraction;
        glutPostRedisplay();    // update obyek
    case GLUT_KEY_F1:
        numSides++;
        numRings++;
        glutPostRedisplay();    // update obyek
        break;
    case GLUT_KEY_F2:
        numSides--;
        numRings--;
        if (numSides < 3) numSides = 3;  // jangan biarkan kurang dari 3
        if (numRings < 3) numRings = 3;  // jangan biarkan kurang dari 3
        glutPostRedisplay();    // update obyek
        break;
    }
}

// timer untuk animasi rotasi obyek
void timer(int value)
{
    objectAngleX += 0.2f;     // kecepatan rotasi obyek sebesar 0.2 derajat
    objectAngleY += 0.2f;     // kecepatan rotasi obyek sebesar 0.2 derajat
    if (objectAngleX > 360.0f) objectAngleX -= 360.0f;   // reset sudut rotasi bila lebih dari 360 derajat
    if (objectAngleY > 360.0f) objectAngleY -= 360.0f;   // reset sudut rotasi bila lebih dari 360 derajat

    glutPostRedisplay();    // update obyek
    glutTimerFunc(16, timer, 0); // panggil kembali fungsi timer setelah 16ms
}

// fungsi utama untuk menjalankan OpenGL
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);    // set display mode menjadi RGB, Depth dan Double Buffer
    glutInitWindowSize(500, 500);            // set ukuran layar
    glutInitWindowPosition(100, 100);        // set posisi layar
    glutCreateWindow("AULIA IRFAN HAKIM / 2200018329 - POST 10");            // set judul layar
    init();                    // panggil fungsi init
    glutDisplayFunc(display);            // panggil fungsi display
    glutReshapeFunc(reshape);            // panggil fungsi reshape
    glutSpecialFunc(keyboard);            // panggil fungsi keyboard
    glutTimerFunc(0, timer, 0);            // panggil fungsi timer

    glutMainLoop();                // masuk ke dalam loop glut
    return 0;
}

