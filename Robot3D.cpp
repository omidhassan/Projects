/*******************************************************************
		   Hierarchical Multi-Part Model Example
********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gl/glut.h>
#include <utility>
#include <vector>
#include "VECTOR3D.h"
#include "cube.h"
#include "QuadMesh.h"

const int vWidth = 650;    // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels

// Note how everything depends on robot body dimensions so that can scale entire robot proportionately
// just by changing robot body scale
float robotBodyWidth = 4.0;
float robotBodyLength = 2.0;
float robotBodyDepth = 4.0;
float foward = 0.0;
float backward = 0.0;
static float wheelangle = 0.0;
static float wheelangle2 = 0.0;
float weaponspin = 0.0;
float timer = 0;


// Control Robot body rotation on base
float robotAngle = 0.0;
float robotAngle2 = 0.0;

// Spin Cube Mesh
float cubeAngle = 0.0;

// Lighting/shading and material properties for robot - upcoming lecture - just copy for now
// Robot RGBA material properties (NOTE: we will learn about this later in the semester)
GLfloat robotBody_mat_ambient[] = { 0.0f,0.0f,5.0f,0.0f };
GLfloat robotBody_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat robotBody_mat_diffuse[] = { 0.1f,0.35f,0.1f,1.0f };
GLfloat robotBody_mat_shininess[] = { 32.0F };

GLfloat wheel_mat_ambient[] = { 3.0f, 0.0f, -3.0f, 0.0f };
GLfloat weapon_mat_ambient[] = { 0.0f, 5.0f, 5.0f, 5.0f };

GLfloat robotArm_mat_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 0.55f };
GLfloat robotArm_mat_diffuse[] = { 0.5f,0.0f,0.0f,1.0f };
GLfloat robotArm_mat_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
GLfloat robotArm_mat_shininess[] = { 32.0F };

GLfloat gun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat gun_mat_diffuse[] = { 0.01f,0.0f,0.01f,0.01f };
GLfloat gun_mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat gun_mat_shininess[] = { 100.0F };

GLfloat robotLowerBody_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat robotLowerBody_mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat robotLowerBody_mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
GLfloat robotLowerBody_mat_shininess[] = { 76.8F };


// Light properties
GLfloat light_position0[] = { -4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_position1[] = { 4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };


// Mouse button
int currentButton;

// A template cube mesh
CubeMesh* cubeMesh = createCubeMesh();

// A flat open mesh
QuadMesh* groundMesh = NULL;

// Structure defining a bounding box, currently unused
typedef struct BoundingBox {
	VECTOR3D min;
	VECTOR3D max;
} BBox;

// Default Mesh Size
int meshSize = 16;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void animationHandler(int param);
void animationHandler2(int param);
void animationHandler3(int param);
void drawRobot();
void drawBody();
void drawWheel1();
void drawWheel2();
void drawFrontPiece();
void drawFrontPieceBottom();
void drawWeapon();

int main(int argc, char** argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(vWidth, vHeight);
	glutInitWindowPosition(200, 30);
	glutCreateWindow("3D Hierarchical Example");

	// Initialize GL
	initOpenGL(vWidth, vHeight);

	// Register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotionHandler);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(functionKeys);

	// Start event loop, never returns
	glutMainLoop();

	return 0;
}


// Set up OpenGL. For viewport and projection setup see reshape(). 
void initOpenGL(int w, int h)
{
	// Set up and enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);   // This second light is currently off

	//glEnable(GL_COLOR_MATERIAL);

	// Other OpenGL setup
	glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
	glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
	glClearColor(0.4F, 0.4F, 0.4F, 0.0F);  // Color and depth for glClear
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// Other initializatuion
	// Set up ground quad mesh
	VECTOR3D origin = VECTOR3D(-16.0f, 0.0f, 16.0f);
	VECTOR3D dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
	VECTOR3D dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
	groundMesh = new QuadMesh(meshSize, 32.0);
	groundMesh->InitMesh(meshSize, origin, 32.0, 32.0, dir1v, dir2v);

	VECTOR3D ambient = VECTOR3D(0.0f, 0.05f, 0.0f);
	VECTOR3D diffuse = VECTOR3D(0.4f, 0.8f, 0.4f);
	VECTOR3D specular = VECTOR3D(0.04f, 0.04f, 0.04f);
	float shininess = 0.2;
	groundMesh->SetMaterial(ambient, diffuse, specular, shininess);

}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	// Create Viewing Matrix V
	// Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
	gluLookAt(0.0, 10.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(-20.0, 2.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Draw Robot

	// Apply modelling transformations M to move robot
	// Current transformation matrix is set to IV, where I is identity matrix
	// CTM = IV
	drawRobot();

	// Example of drawing a mesh (closed cube mesh)
	glPushMatrix();
	// spin cube
	glTranslatef(20.0, 0, 3.0);
	glRotatef(cubeAngle, 0.0, 1.0, 0.0);
	glTranslatef(-8.0, 0, -3.0);
	// position and draw cube
	glTranslatef(2.0, 0, 3.0);
	//glutSolidCube(10);
	drawCubeMesh(cubeMesh);
	glPopMatrix();

	// Draw ground
	glPushMatrix();
	glTranslatef(0.0, -5.0, 0.0);
	groundMesh->DrawMesh(meshSize);
	glPopMatrix();

	glutSwapBuffers();   // Double buffering, swap buffers
}

bool weapon = false;
float foward2 = 0.0;
float backward2 = 0.0;

void drawRobot()
{
	glPushMatrix();

	glTranslatef(0.0 + foward - backward, 0.125, robotBodyDepth - 2 + foward2 - backward2);
	glRotatef(robotAngle, 0.0, 1.0, 0.0);
	glTranslatef(0.0 - foward + backward, -0.125, -robotBodyDepth + 2 - foward2 + backward2);
	
	glPushMatrix();
	glTranslatef(0.0 + foward - backward, 0.125, 2.0 + foward2 - backward2);
	drawBody();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.75 * robotBodyWidth + foward - backward, 0.0, robotBodyDepth-2 + foward2 - backward2);
	glRotatef(wheelangle, 1, 0, 0);
	drawWheel1();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.75 * robotBodyWidth + foward - backward, 0.0, robotBodyDepth - 2 + foward2 - backward2);
	glRotatef(wheelangle2, 1, 0, 0);
	drawWheel2();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0 + foward - backward, 0.30 * robotBodyWidth, robotBodyDepth +0.5 + foward2 - backward2);
	drawFrontPiece();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0 + foward - backward, -0.2 * robotBodyWidth, robotBodyDepth + 0.5 + foward2 - backward2);
	drawFrontPieceBottom();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0 + foward - backward, 0.05 * robotBodyWidth, robotBodyLength + 3.5 + foward2 - backward2);
	glRotatef(weaponspin, 0.0, 1.0, 0.0);
	drawWeapon();
	glPopMatrix();
	

	glPopMatrix();
}


void drawBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();

	glScalef(robotBodyWidth, robotBodyLength, robotBodyDepth);
	glutSolidCube(1.0);
	
	glPopMatrix();

	
}

void drawWheel1()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, wheel_mat_ambient);

	GLUquadricObj* Wheel1;
	Wheel1 = gluNewQuadric();
	gluQuadricDrawStyle(Wheel1, GLU_LINE);

	GLUquadricObj* Disk1;
	Disk1 = gluNewQuadric();
	gluQuadricDrawStyle(Disk1, GLU_LINE);

	glPushMatrix();
	glRotatef(270.0, 0.0, 90.0, 0.0);
	gluCylinder(Wheel1, 1.25, 1.25, 1.0, 20.0, 200.0);
	
	glPopMatrix();

	glPushMatrix();
	glRotatef(270.0, 0.0, 90.0, 0.0);
	gluDisk(Disk1, 0.0, 1.25, 20.0, 200.0);
	glPopMatrix();
}
void drawWheel2()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, wheel_mat_ambient);

	GLUquadricObj* Wheel2;
	Wheel2 = gluNewQuadric();
	gluQuadricDrawStyle(Wheel2, GLU_LINE);

	GLUquadricObj* Disk2;
	Disk2 = gluNewQuadric();
	gluQuadricDrawStyle(Disk2, GLU_LINE);

	glPushMatrix();
	glRotatef(90.0, 0.0, 90.0, 0.0);
	gluCylinder(Wheel2, 1.25, 1.25, 1.0, 20.0, 200.0);

	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0, 0.0, 90.0, 0.0);
	gluDisk(Disk2, 0.0, 1.25, 20.0, 200.0);
	glPopMatrix();
}

void drawFrontPiece()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, wheel_mat_ambient);


	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(robotBodyLength * 0.25, 0, 0.5);
	glVertex3f(robotBodyLength, 0, -0.5);
	glVertex3f(-robotBodyLength , 0, -0.5);
	glVertex3f(-robotBodyLength * 0.25, 0, 0.5);

	glVertex3f(robotBodyLength * 0.25, -0.25, 0.5);
	glVertex3f(robotBodyLength, -0.25, -0.5);
	glVertex3f(-robotBodyLength, -0.25, -0.5);
	glVertex3f(-robotBodyLength * 0.25, -0.25, 0.5);

	glVertex3f(robotBodyLength * 0.25, 0, 0.5);
	glVertex3f(robotBodyLength * 0.25, -0.25, 0.5);
	glVertex3f(robotBodyLength * 0.25, -0.25, 0.5);
	glVertex3f(robotBodyLength * 0.25, 0, 0.5);

	glVertex3f(robotBodyLength * 0.25, 0, 0.5);
	glVertex3f(robotBodyLength * 0.25, -0.25, 0.5);
	glVertex3f(robotBodyLength, -0.25, -0.5);
	glVertex3f(robotBodyLength, 0, -0.5);

	glVertex3f(-robotBodyLength * 0.25, 0, 0.5);
	glVertex3f(-robotBodyLength * 0.25, -0.25, 0.5);
	glVertex3f(-robotBodyLength, -0.25, -0.5);
	glVertex3f(-robotBodyLength, 0, -0.5);
	
	glEnd();
	glPushMatrix();

	glScalef(0.25, 2, 0.25);
	glRotatef(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
	glPopMatrix();
	


	glPopMatrix();

}

void drawFrontPieceBottom()
{
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(robotBodyLength * 0.25, 0, 0.5);
	glVertex3f(robotBodyLength, 0, -0.5);
	glVertex3f(-robotBodyLength, 0, -0.5);
	glVertex3f(-robotBodyLength * 0.25, 0, 0.5);

	glVertex3f(robotBodyLength * 0.25, -0.25, 0.5);
	glVertex3f(robotBodyLength, -0.25, -0.5);
	glVertex3f(-robotBodyLength, -0.25, -0.5);
	glVertex3f(-robotBodyLength * 0.25, -0.25, 0.5);

	glVertex3f(robotBodyLength * 0.25, 0, 0.5);
	glVertex3f(robotBodyLength * 0.25, -0.25, 0.5);
	glVertex3f(robotBodyLength * 0.25, -0.25, 0.5);
	glVertex3f(robotBodyLength * 0.25, 0, 0.5);

	glVertex3f(robotBodyLength * 0.25, 0, 0.5);
	glVertex3f(robotBodyLength * 0.25, -0.25, 0.5);
	glVertex3f(robotBodyLength, -0.25, -0.5);
	glVertex3f(robotBodyLength, 0, -0.5);

	glVertex3f(-robotBodyLength * 0.25, 0, 0.5);
	glVertex3f(-robotBodyLength * 0.25, -0.25, 0.5);
	glVertex3f(-robotBodyLength, -0.25, -0.5);
	glVertex3f(-robotBodyLength, 0, -0.5);

	glEnd();

	glPopMatrix();
}

void drawWeapon()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, weapon_mat_ambient);
	GLUquadricObj* Disk2;
	Disk2 = gluNewQuadric();
	gluQuadricDrawStyle(Disk2, GLU_LINE);
	glPushMatrix();
	glRotatef(90.0, 1, 0, 0);
	glScalef(1.5, 1.5, 0);
	gluDisk(Disk2, 0, 1.0, 20, 200);
	glPopMatrix();
}

// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
	// Set up viewport, projection, then change to modelview matrix mode - 
	// display function will then set up camera and do modeling transforms.
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 40.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
	gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

bool stop = false;
bool stop2 = false;
bool left = false;
bool right = false;

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 't':

		break;
	case 'f':
		foward2 += 1 * cosf(-robotAngle * 3.1459 / 180);
		foward -= 1 * sinf(-robotAngle * 3.1459 / 180);
		glutTimerFunc(10, animationHandler, 0);
		break;
	case 'F':
		foward += 1;
		wheelangle += 2;
		break;
	case 'b':
		backward2 += 1 * cosf(-robotAngle * 3.1459 / 180);
		backward -= 1 * sinf(-robotAngle * 3.1459 / 180);
		stop2 = true;
		glutTimerFunc(10, animationHandler, 0);
		break;
	case 'B':
		backward += 1;
		break;
	case 'l':
		left = true;
		robotAngle += 2.0;
		glutTimerFunc(10, animationHandler3, 0);
		break;
	case 'L':
		robotAngle += 2.0;
		break;
	case 'r':
		right = true;
		robotAngle -= 2.0;
		//printf("%d ", sinf(robotAngle));
		glutTimerFunc(10, animationHandler3, 0);
		break;
	case ' ':
		if (weaponspin)
		{
			weaponspin = false;
			weapon = false;
		}
		else
		{
			weaponspin = true;
			weapon = true;
		}
		glutTimerFunc(10, animationHandler2, 0);
		break;
	case 's':
		glutTimerFunc(10, animationHandler, 0);
		break;
	case 'S':
		stop = true;
		break;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}


void animationHandler(int param)
{
	if (timer == 50)
	{
		timer = 0;
		stop = true;
	}
	if (!stop)
	{
		if (stop2)
		{
			wheelangle -= 1;
			wheelangle2 -= 1;
		}
		else
		{
			wheelangle += 1;
			wheelangle2 += 1;
		}
		timer += 1;
		glutPostRedisplay();
		glutTimerFunc(10, animationHandler, 0);
	}
	else
	{
		stop2 = false;
		stop = false;
	}
}

void animationHandler2(int param)
{
	if (weapon)
	{
		weaponspin += 2;
		glutPostRedisplay();
		glutTimerFunc(10, animationHandler2, 0);
	}
}
void animationHandler3(int param)
{
	if (timer == 100)
	{
		timer = 0;
		left = false;
		right = false;
	}
	if (left)
	{
		wheelangle2 += 2;
		timer += 1;
		glutTimerFunc(10, animationHandler3, 0);
		//left = false;
		//wheelangle2 -= 2;
	}
	if (right)
	{
		//wheelangle -= 2;
		wheelangle += 2;
		timer += 1;
		glutTimerFunc(10, animationHandler3, 0);
	}

}



// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
	// Help key
	if (key == GLUT_KEY_F1)
	{
		printf("This is the Help Screen\n");
		printf("-----------------------\n");
		printf("Press f to move foward\n");
		printf("Press b to move backward\n");
		printf("Press l to turn left\n");
		printf("Press r to turn right\n");
		printf("Press space bar to activate and deactivate the weapon\n");
	}
	// Do transformations with arrow keys
	//else if (...)   // GLUT_KEY_DOWN, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_LEFT
	//{
	//}

	glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
	currentButton = button;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			;

		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			;
		}
		break;
	default:
		break;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
	if (currentButton == GLUT_LEFT_BUTTON)
	{
		;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}

