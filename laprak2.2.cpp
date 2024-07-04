// kode dasar untuk semua praktikum grafika komputer
// adhi prahara. 2018

// deklarasikan semua header disini
#include <windows.h>
#include <GL/glut.h>

#include <stdio.h>
#include <iostream>

int SCREEN_WIDTH = 400; 
int SCREEN_HEIGHT = 400;

// inisialisasi variabel untuk transformasi seperti translasi, rotasi atau scaling
float angle = 0.0f;                    // sudut transformasi kamera
float posX = 0.0f, rotX =  0.0f;    // posisi kamera di sumbu X
float posY = 0.0f, rotY =  0.0f;    // posisi kamera di sumbu Y
float posZ = 5.0f, rotZ = -1.0f;    // posisi kamera di sumbu Z

float objectAngleX = 0.0f;            // sudut tranformasi obyek di sumbu X
float objectAngleY = 0.0f;            // sudut tranformasi obyek di sumbu Y
float objectAngleZ = 0.0f;            // sudut tranformasi obyek di sumbu Z

// container untuk membuat tipe data 3D (X, Y, Z) 
struct Vec3{ 
    float X; float Y; float Z;
    Vec3(float x, float y, float z) { X = x; Y = y; Z = z; }
    //
    Vec3() { }
    ~Vec3() { }
}; 

// fungsi untuk menggambar garis dengan algoritma DDA bila terhadap X
void lineDDAX(Vec3 point1, Vec3 point2)
{
    // hitung gradient garis m
    int dY = point2.Y - point1.Y;
    int dX = point1.X - point2.X;
    float m = (float)dY / dX;
    float im = 1.0f/m;
    
    // mulai menggambar titik-titik
    glBegin(GL_POINTS);
    // koordinat titik awal
    glVertex3f(point1.X, point1.Y, point1.Z);
    float pX = point1.X, pY = point1.Y, pZ = point1.Z;
    
    // kenaikan terhadap X
    for (int i = point1.X; i < point2.X; i++)
    {
        pX = pX + 1; // Xn+1 = Xn + 1
        pY = pY + m; // Yn+1 = Yn + m
        glVertex3f(pX, pY, pZ);
    }
    // koordinat titik akhir
    glVertex3f(point2.X, point2.Y, point2.Z);
    glEnd();
}

// fungsi untuk menggambar garis dengan algoritma DDA bila terhadap Y
void lineDDAY(Vec3 point1, Vec3 point2)
{
    // hitung gradient garis m
    int dY = point2.Y - point1.Y;
    int dX = point2.X - point1.X;
    float m = (float)dY / dX;
    float im = 1.0f/m;
    
    // mulai menggambar titik-titik
    glBegin(GL_POINTS);
    // koordinat titik awal
    glVertex3f(point1.X, point1.Y, point1.Z);
    float pX = point1.X, pY = point1.Y, pZ = point1.Z;
    
    // kenaikan terhadap Y
    for (int i = point1.Y; i < point2.Y; i++)
    {
        pX = pX + im; // Xn+1 = Xn + 1/m
        pY = pY + 1; // Yn+1 = Yn + 1
        glVertex3f(pX, pY, pZ);
    }
    // koordinat titik akhir
    glVertex3f(point2.X, point2.Y, point2.Z);
    glEnd();
}

// fungsi untuk menggambar garis dengan algoritma DDA
void lineDDA(Vec3 point1, Vec3 point2)
{
    // hitung selisih panjang
    int dY = point2.Y - point1.Y;
    int dX = point2.X - point1.X;
    // bila deltaY lebih pendek dari deltaX
    if (abs(dY) < abs(dX)) 
    {
        if (point1.X < point2.X) // bila X1 < X2
            lineDDAX(point1, point2);
        else // bila X1 > X2 maka dibalik
            lineDDAX(point2, point1);
    }
    else // bila deltaY lebih panjang dari deltaX
    {
        if (point1.Y < point2.Y) // bila Y1 < Y2
            lineDDAY(point1, point2);
        else // bila Y1 > Y2 maka dibalik
            lineDDAY(point2, point1);
    }
}

// fungsi untuk menggambar obyek
void drawObject()
{
    glPushMatrix();
    // operasi transformasi rotasi obyek ke arah kanan-kiri
    glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);
    glPushMatrix();
    // operasi transformasi rotasi obyek ke arah atas-bawah
    glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);
    
    // set warna obyek ke warna hijau (0.0f, 1.0f, 0.0f)
    glColor3f(0.0f, 1.0f, 0.0f);
    
    // gambar sumbu
    Vec3 sbY1 = Vec3( 0.0f,-300.0f, 0.0f);
    Vec3 sbY2 = Vec3( 0.0f, 300.0f, 0.0f);
    Vec3 sbX1 = Vec3(-300.0f, 0.0f, 0.0f);
    Vec3 sbX2 = Vec3( 300.0f, 0.0f, 0.0f);
    lineDDA(sbX1, sbX2);
    lineDDA(sbY1, sbY2);

    // gambar segitiga
    Vec3 point1 = Vec3(0.0f, 100.0f, 0.0f);
    Vec3 point2 = Vec3(-100.0f, -100.0f, 0.0f);
    Vec3 point3 = Vec3(100.0f, -100.0f, 0.0f);
    lineDDA(point1, point2);
    lineDDA(point2, point3);
    lineDDA(point3, point1);

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
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);                // mengaktifkan depth buffer
    glMatrixMode(GL_PROJECTION);        
    glLoadIdentity();
    //gluPerspective(45.0, 1.0, 1.0, 100.0);    
    glOrtho((GLfloat)-SCREEN_WIDTH/2, (GLfloat)SCREEN_WIDTH/2, (GLfloat)-SCREEN_HEIGHT/2, (GLfloat)SCREEN_HEIGHT/2, 1.0, 100.0); // set proyeksi ke perspektif
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
    glOrtho((GLfloat)-w/2, (GLfloat)w/2, (GLfloat)-h/2, (GLfloat)h/2, 1.0, 100.0); // set proyeksi ke perspektif
    glMatrixMode(GL_MODELVIEW);
}

// fungsi ini digunakan untuk mengatur input keyboard
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // bila menekan tombol ESC, maka keluar program
        exit(0);
        break;
    default:
        break;
    }
}

// fungsi ini digunakan untuk mengatur input tombol spesial (keyboard)
void specialKeyboard(int key, int x, int y)
{
    // ketahui nilai tombol panah yang ditentukan user
    switch (key)
    {
    case GLUT_KEY_LEFT:    // transformasi rotasi sumbu Y obyek ke arah kiri
        objectAngleY -= 5.0f;
        break;
    case GLUT_KEY_RIGHT: // transformasi rotasi sumbu Y obyek ke arah kanan
        objectAngleY += 5.0f;
        break;
    case GLUT_KEY_UP:    // transformasi rotasi sumbu X obyek ke arah atas
        objectAngleX -= 5.0f;
        break;
    case GLUT_KEY_DOWN:    // transformasi rotasi sumbu X obyek ke arah bawah
        objectAngleX += 5.0f;
        break;
    default:
        break;
    }
    glutPostRedisplay(); // panggil display function untuk perbaharui gambar di layar
}

// fungsi utama
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Algoritma DDA");

    init(); // panggil fungsi inisialisasi untuk setting awal

    // beberapa fungsi callback yang digunakan
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);

    glutMainLoop();
    return 0;
}

