// kode dasar untuk semua praktikum grafika komputer
// adhi prahara. 2018

// deklarasikan semua header disini
#include <windows.h>
#include <math.h>
#include <GL/glut.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 480
#define NUM_BOXES 20  // jumlah kotak kecil yang mengelilingi lingkaran

// inisialisasi variabel untuk transformasi seperti translasi, rotasi atau scaling
float angle = 0.0f;					// sudut transformasi kamera
float posX = 0.0f, rotX =  0.0f;	// posisi kamera di sumbu X
float posY = 0.0f, rotY =  0.0f;	// posisi kamera di sumbu Y
float posZ = 5.0f, rotZ = -1.0f;	// posisi kamera di sumbu Z

float objectAngleX = 0.0f;			// sudut tranformasi obyek di sumbu X
float objectAngleY = 0.0f;			// sudut tranformasi obyek di sumbu Y
float objectAngleZ = 0.0f;			// sudut tranformasi obyek di sumbu Z

// container untuk membuat tipe data 3D (X, Y, Z)
struct Vec3
{
	float X; float Y; float Z;
	Vec3(float x, float y, float z) { X = x; Y = y; Z = z; }
	//
	Vec3() { }
	~Vec3() { }
};

// menggambar setiap titik kontrol kurva
void markPoint(Vec3 points, Vec3 colors, float width)
{
	// tandai setiap titik dengan warna
	glPushMatrix();
	glColor3f(colors.X, colors.Y, colors.Z);

	glBegin(GL_QUADS);
	glVertex3f(points.X - width, points.Y - width, points.Z);
	glVertex3f(points.X + width, points.Y - width, points.Z);
	glVertex3f(points.X + width, points.Y + width, points.Z);
	glVertex3f(points.X - width, points.Y + width, points.Z);
	glEnd();

	glPopMatrix();
}

// fungsi untuk menghitung inverse matriks ordo 4x4
bool inverse(float inMat[16], float outMat[16])
{
	float inv[16], det;
	int i;

	inv[0] = 
		inMat[5] * inMat[10] * inMat[15] -
		inMat[5] * inMat[11] * inMat[14] -
		inMat[9] * inMat[6] * inMat[15] +
		inMat[9] * inMat[7] * inMat[14] +
		inMat[13] * inMat[6] * inMat[11] -
		inMat[13] * inMat[7] * inMat[10];

	inv[4] = 
	   -inMat[4] * inMat[10] * inMat[15] +
		inMat[4] * inMat[11] * inMat[14] +
		inMat[8] * inMat[6] * inMat[15] -
		inMat[8] * inMat[7] * inMat[14] -
		inMat[12] * inMat[6] * inMat[11] +
		inMat[12] * inMat[7] * inMat[10];

	inv[8] = 
		inMat[4] * inMat[9] * inMat[15] -
		inMat[4] * inMat[11] * inMat[13] -
		inMat[8] * inMat[5] * inMat[15] +
		inMat[8] * inMat[7] * inMat[13] +
		inMat[12] * inMat[5] * inMat[11] -
		inMat[12] * inMat[7] * inMat[9];

	inv[12] = 
	   -inMat[4] * inMat[9] * inMat[14] +
		inMat[4] * inMat[10] * inMat[13] +
		inMat[8] * inMat[5] * inMat[14] -
		inMat[8] * inMat[6] * inMat[13] -
		inMat[12] * inMat[5] * inMat[10] +
		inMat[12] * inMat[6] * inMat[9];

	inv[1] = 
	   -inMat[1] * inMat[10] * inMat[15] +
		inMat[1] * inMat[11] * inMat[14] +
		inMat[9] * inMat[2] * inMat[15] -
		inMat[9] * inMat[3] * inMat[14] -
		inMat[13] * inMat[2] * inMat[11] +
		inMat[13] * inMat[3] * inMat[10];

	inv[5] = 
		inMat[0] * inMat[10] * inMat[15] -
		inMat[0] * inMat[11] * inMat[14] -
		inMat[8] * inMat[2] * inMat[15] +
		inMat[8] * inMat[3] * inMat[14] +
		inMat[12] * inMat[2] * inMat[11] -
		inMat[12] * inMat[3] * inMat[10];

	inv[9] = 
	   -inMat[0] * inMat[9] * inMat[15] +
		inMat[0] * inMat[11] * inMat[13] +
		inMat[8] * inMat[1] * inMat[15] -
		inMat[8] * inMat[3] * inMat[13] -
		inMat[12] * inMat[1] * inMat[11] +
		inMat[12] * inMat[3] * inMat[9];

	inv[13] = 
		inMat[0] * inMat[9] * inMat[14] -
		inMat[0] * inMat[10] * inMat[13] -
		inMat[8] * inMat[1] * inMat[14] +
		inMat[8] * inMat[2] * inMat[13] +
		inMat[12] * inMat[1] * inMat[10] -
		inMat[12] * inMat[2] * inMat[9];

	inv[2] = 
		inMat[1] * inMat[6] * inMat[15] -
		inMat[1] * inMat[7] * inMat[14] -
		inMat[5] * inMat[2] * inMat[15] +
		inMat[5] * inMat[3] * inMat[14] +
		inMat[13] * inMat[2] * inMat[7] -
		inMat[13] * inMat[3] * inMat[6];

	inv[6] = 
	   -inMat[0] * inMat[6] * inMat[15] +
		inMat[0] * inMat[7] * inMat[14] +
		inMat[4] * inMat[2] * inMat[15] -
		inMat[4] * inMat[3] * inMat[14] -
		inMat[12] * inMat[2] * inMat[7] +
		inMat[12] * inMat[3] * inMat[6];

	inv[10] = 
		inMat[0] * inMat[5] * inMat[15] -
		inMat[0] * inMat[7] * inMat[13] -
		inMat[4] * inMat[1] * inMat[15] +
		inMat[4] * inMat[3] * inMat[13] +
		inMat[12] * inMat[1] * inMat[7] -
		inMat[12] * inMat[3] * inMat[5];

	inv[14] = 
	   -inMat[0] * inMat[5] * inMat[14] +
		inMat[0] * inMat[6] * inMat[13] +
		inMat[4] * inMat[1] * inMat[14] -
		inMat[4] * inMat[2] * inMat[13] -
		inMat[12] * inMat[1] * inMat[6] +
		inMat[12] * inMat[2] * inMat[5];

	inv[3] = 
	   -inMat[1] * inMat[6] * inMat[11] +
		inMat[1] * inMat[7] * inMat[10] +
		inMat[5] * inMat[2] * inMat[11] -
		inMat[5] * inMat[3] * inMat[10] -
		inMat[9] * inMat[2] * inMat[7] +
		inMat[9] * inMat[3] * inMat[6];

	inv[7] = 
		inMat[0] * inMat[6] * inMat[11] -
		inMat[0] * inMat[7] * inMat[10] -
		inMat[4] * inMat[2] * inMat[11] +
		inMat[4] * inMat[3] * inMat[10] +
		inMat[8] * inMat[2] * inMat[7] -
		inMat[8] * inMat[3] * inMat[6];

	inv[11] = 
	   -inMat[0] * inMat[5] * inMat[11] +
		inMat[0] * inMat[7] * inMat[9] +
		inMat[4] * inMat[1] * inMat[11] -
		inMat[4] * inMat[3] * inMat[9] -
		inMat[8] * inMat[1] * inMat[7] +
		inMat[8] * inMat[3] * inMat[5];

	inv[15] = 
		inMat[0] * inMat[5] * inMat[10] -
		inMat[0] * inMat[6] * inMat[9] -
		inMat[4] * inMat[1] * inMat[10] +
		inMat[4] * inMat[2] * inMat[9] +
		inMat[8] * inMat[1] * inMat[6] -
		inMat[8] * inMat[2] * inMat[5];

	det = inMat[0] * inv[0] + inMat[1] * inv[4] + inMat[2] * inv[8] + inMat[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		outMat[i] = inv[i] * det;

	return true;
}

// fungsi untuk merubah matriks openGL (row major) ke matriks vektor (column major)
void getColumnMajor(float inMatrix[16], float outMatrix[16])
{
	// kolom 1
	outMatrix[0] = inMatrix[0];
	outMatrix[1] = inMatrix[4];
	outMatrix[2] = inMatrix[8];
	outMatrix[3] = inMatrix[12];

	// kolom 2
	outMatrix[4] = inMatrix[1];
	outMatrix[5] = inMatrix[5];
	outMatrix[6] = inMatrix[9];
	outMatrix[7] = inMatrix[13];

	// kolom 3
	outMatrix[8] = inMatrix[2];
	outMatrix[9] = inMatrix[6];
	outMatrix[10] = inMatrix[10];
	outMatrix[11] = inMatrix[14];

	// kolom 4
	outMatrix[12] = inMatrix[3];
	outMatrix[13] = inMatrix[7];
	outMatrix[14] = inMatrix[11];
	outMatrix[15] = inMatrix[15];
}

// fungsi untuk mendapatkan matriks vektor (column major) ke matriks openGL (row major)
void getRowMajor(float inMatrix[16], float outMatrix[16])
{
	// baris 1
	outMatrix[0] = inMatrix[0];
	outMatrix[4] = inMatrix[1];
	outMatrix[8] = inMatrix[2];
	outMatrix[12] = inMatrix[3];

	// baris 2
	outMatrix[1] = inMatrix[4];
	outMatrix[5] = inMatrix[5];
	outMatrix[9] = inMatrix[6];
	outMatrix[13] = inMatrix[7];

	// baris 3
	outMatrix[2] = inMatrix[8];
	outMatrix[6] = inMatrix[9];
	outMatrix[10] = inMatrix[10];
	outMatrix[14] = inMatrix[11];

	// baris 4
	outMatrix[3] = inMatrix[12];
	outMatrix[7] = inMatrix[13];
	outMatrix[11] = inMatrix[14];
	outMatrix[15] = inMatrix[15];
}

// fungsi ini akan dipanggil setiap kali layar perlu di re-draw ulang
void display()
{
	// clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// set kamera
	gluLookAt(posX, posY, posZ, rotX, rotY, rotZ, 0.0f, 1.0f, 0.0f);

	// mulai gambar ulang scene
	glPushMatrix();

	// letakkan sumbu di titik (0, 0, 0)
	glTranslatef(0.0f, 0.0f, 0.0f);

	// buat beberapa transformasi terhadap obyek
	glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);	// rotasi sumbu X
	glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);	// rotasi sumbu Y
	glRotatef(objectAngleZ, 0.0f, 0.0f, 1.0f);	// rotasi sumbu Z

	// gambar obyek disini
	//int num_points = sizeof(points) / sizeof(points[0]);

	// gambar lingkaran
	int num_segments = 100;
	float radius = 1.0f;
	glColor3f(0.0f, 1.0f, 0.0f); // warna lingkaran
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < num_segments; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);
		glVertex2f(x, y);
	}
	glEnd();

	// gambar kotak kecil mengelilingi lingkaran
	for (int i = 0; i < NUM_BOXES; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(NUM_BOXES);
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);
		markPoint(Vec3(x, y, 0.0f), Vec3(1.0f, 0.0f, 0.0f), 0.05f); // warna merah untuk kotak kecil
	}

	glPopMatrix();
	glutSwapBuffers();
}

// inisialisasi parameter dan variabel openGL
void initialize()
{
	// set background warna hitam dan kedalaman
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	// enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// enable smooth shading
	glShadeModel(GL_SMOOTH);

	// aspek rasio
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// fungsi ini akan dipanggil setiap kali ukuran layar berubah
void reshape(int w, int h)
{
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// fungsi ini akan dipanggil setiap kali ada input dari keyboard
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': // rotasi ke atas
            objectAngleX -= 5.0f;
            break;
        case 's': // rotasi ke bawah
            objectAngleX += 5.0f;
            break;
        case 'a': // rotasi ke kiri
            objectAngleY -= 5.0f;
            break;
        case 'd': // rotasi ke kanan
            objectAngleY += 5.0f;
            break;
        case 27: // Esc untuk keluar
            exit(0);
    }
    glutPostRedisplay(); // meminta OpenGL untuk merender ulang layar
}


// fungsi ini akan dipanggil setiap kali ada input dari mouse
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			// aksi jika tombol kiri mouse ditekan
		}
		break;
	default:
		break;
	}
}

// fungsi ini akan dipanggil untuk mengupdate animasi
void timer(int value)
{
	objectAngleZ += 1.0f;
	if (objectAngleZ > 360.0f)
		objectAngleZ -= 360.0f;

	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);  // panggil fungsi ini lagi setelah 16 ms (60 fps)
}

// fungsi utama
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("AULIA IRFAN HAKIM / 2200018329 - LP 7");

	initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardFunc(handleKeypress);
	glutMouseFunc(mouse);
//	glutTimerFunc(0, timer, 0);
	glutMainLoop();

	return 0;
}

