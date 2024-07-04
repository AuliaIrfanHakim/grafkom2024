// kode dasar untuk semua praktikum grafika komputer
// adhi prahara. 2018

// deklarasikan semua header disini
#include <windows.h>
#include <GL/glut.h>
#include <cmath>

// inisialisasi variabel untuk transformasi seperti translasi, rotasi atau scaling
float angle = 0.0f;                 // sudut transformasi kamera
float posX = 0.0f, rotX =  0.0f;    // posisi kamera di sumbu X
float posY = 0.0f, rotY =  0.0f;    // posisi kamera di sumbu Y
float posZ = 5.0f, rotZ = -1.0f;    // posisi kamera di sumbu Z

float objectAngleX = 0.0f;          // sudut tranformasi obyek di sumbu X
float objectAngleY = 0.0f;          // sudut tranformasi obyek di sumbu Y
float objectAngleZ = 0.0f;          // sudut tranformasi obyek di sumbu Z

// fungsi untuk menggambar obyek prisma segi enam
void drawObject()
{
    // obyek bisa dimasukkan diantara glPushMatrix() dan glPopMatrix() 
    // fungsinya agar obyek tidak terpengaruh atau mempengaruhi obyek lain
    // saat diwarnai, ditransformasi dan sebagainya
    glPushMatrix();

    // operasi transformasi rotasi obyek ke arah kanan-kiri
    glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);

    glPushMatrix();

    // operasi transformasi rotasi obyek ke arah atas-bawah
    glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);

    // set warna obyek ke warna coklat
    glColor3f(0.55f, 0.27f, 0.07f); // Saddle Brown

    // menggambar prisma segi enam
    float radius = 0.5f;
    float height = 2.0f;
    int sides = 6;

    glBegin(GL_QUADS); // sisi-sisi prisma
    for (int i = 0; i < sides; i++)
    {
        float angle1 = 2.0f * M_PI * i / sides;
        float angle2 = 2.0f * M_PI * (i + 1) / sides;

        glVertex3f(radius * cos(angle1), -height / 2, radius * sin(angle1));
        glVertex3f(radius * cos(angle2), -height / 2, radius * sin(angle2));
        glVertex3f(radius * cos(angle2), height / 2, radius * sin(angle2));
        glVertex3f(radius * cos(angle1), height / 2, radius * sin(angle1));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN); // tutup atas prisma
    glVertex3f(0.0f, height / 2, 0.0f);
    for (int i = 0; i <= sides; i++)
    {
        float angle = 2.0f * M_PI * i / sides;
        glVertex3f(radius * cos(angle), height / 2, radius * sin(angle));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN); // tutup bawah prisma
    glVertex3f(0.0f, -height / 2, 0.0f);
    for (int i = 0; i <= sides; i++)
    {
        float angle = 2.0f * M_PI * i / sides;
        glVertex3f(radius * cos(angle), -height / 2, radius * sin(angle));
    }
    glEnd();

    glPopMatrix();

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
    // zoom in
    case GLUT_KEY_PAGE_UP:
        // masukkan perintah disini bila tombol PgUp ditekan
        posX += rotX * fraction;        
        posZ += rotZ * fraction;
        glutPostRedisplay();    // update obyek
        break;
    // zoom out
    case GLUT_KEY_PAGE_DOWN:
        // masukkan perintah disini bila tombol PgDn ditekan
        posX -= rotX * fraction;
        posZ -= rotZ * fraction;
        glutPostRedisplay();    // update obyek
        break;
    }
}

// timer untuk animasi (gunakan bila perlu)
void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(55, timer, 0);
}

// program utama
int main(int argc, char** argv)
{
    // inisialisasi jendela OpenGL
    // GLUT_SINGLE berarti memakai single buffer
    // GLUT_DOUBLE berarti memakai double buffer
    // GLUT_RGB berarti mode tampilan yang dipakai RGB
    // GLUT_RGBA berarti mode tampilan yang dipakai RGBA
    // GLUT_DEPTH berarti memakai depth buffer

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// set ukuran jendela tampilan
	glutInitWindowSize(480, 480);        // besarnya jendela dalam piksel dalam hal ini 480x480
	glutInitWindowPosition(100, 100);    // posisi jendela dilayar komputer dalam piksel
	// judul jendela (wajib diubah dengan informasi NAMA / NIM - JUDUL PRAKTIKUM masing-masing)
	glutCreateWindow("AULIA IRFAN HAKIM / 2200018329 - KODE DASAR PRAKTIKUM GRAFIKA KOMPUTER");

	// panggil fungsi init untuk inisialisasi awal
	init();

	// event handler untuk display, reshape dan keyboard
	glutDisplayFunc(display);   // display
	glutReshapeFunc(reshape);   // reshape
	glutSpecialFunc(keyboard);  // keyboard
	//glutTimerFunc(0, timer, 0); // aktifkan timer bila perlu

	// looping
	glutMainLoop();

	return 0;
}


