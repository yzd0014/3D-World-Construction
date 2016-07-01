// assigment6.cpp : Defines the entry point for the console application.
//Author: Yitong Dai
/* instruction
********************************
movementent for viewpoint:
*look up: w
*look down: s
*rotate left:a
*rotate right: d
----------------------
*move to the direction you face: u
*move to the oppsite direction you face: j
*move to left: h
*move to right: k
********************************
movement for student
*move forward: up arrow
*move backward: down arrow
*rotate counter clockwise: left arrow
*rotate clockwise: right arrow
********************************
L: change the light mode
+: increase light intensity
-: decrease light intensity
0,1,3,4: change the number of polygons(for example, press 3 means that there are 4^3 polygons for a rectangle)
F: enter auto camera mode
G: switch student user can control
V: trun on/off spotlight
*People is seperated from the environment which won't be affected by light.
*there is a ambient light that is always above the student that user can control, which can help user to identify which student is currently being controlled.
*/

#include "stdafx.h"
#include <GL/glut.h>
#include <math.h>
#define PI 3.14
#define front 1
#define back 2
#define left 3
#define right 4
#define bottom 5
#define top 6
#define yes 1
#define no 0
#define student_a 1
#define student_b 2
#define student_c 3

int autoMode = no;
int lightMode = 0;//control which light source will be used
float intensityControl = 1;//control the intensity of two light sourcese 
int numberOfPolygon = 4;//default value is 4^4 polygons for a rectangle 
GLfloat light0_postion[] = { 0,0,0,1 };/*define light0*/
GLfloat light0_diffuse[] = { 1, 1, 1, 1.0 };
GLfloat light1_postion[] = { 400, 100, -650, 1 };/*define light1*/
GLfloat light1_diffuse[] = { 0.8, 0.2, 1, 1.0 };
GLfloat light2_ambient[] = { 0.4, 0.4, 0.4, 1.0 };/*define light2*/
GLfloat light3_position[] = { 0, 0, 0, 1 };//define light3 which is a spotlight
GLfloat light3_diffuse[] = { 1, 1, 1, 1.0 };

GLfloat vertices[][3] = { { -2.0,-2.0,-2.0 },{ 2.0,-2.0,-2.0 },
{ 2.0,2.0,-2.0 },{ -2.0, 2.0,-2.0 },{ -2.0,-2.0, 2.0 },
{ 2.0,-2.0,2.0 },{ 2.0, 2.0, 2.0 },{ -2.0, 2.0, 2.0 } };

GLfloat cameraX[16] = { 1, 0, 0, 1 };//reperesents x aixs of camera
GLfloat cameraY[16] = { 0, 1, 0, 1 };//reperesents y aixs of camera
GLfloat cameraZ[16] = { 0, 0, -1, 1 };//reperesents z aixs of camera

GLfloat viewX = 300;//coordinate of viewpoint
GLfloat viewY = 60;//coordinate of viewpoint
GLfloat viewZ = -100;//coordinate of viewpoint

GLfloat student_a_X = 180;//coordinate of student_a
GLfloat student_a_Z = -750;//coordinate of student
GLfloat a_angel = 0;//angel of student

GLfloat student_b_X = 173;//student_b
GLfloat student_b_Z = -844;
GLfloat b_angel = 0;

GLfloat student_c_X = 200;//student_c
GLfloat student_c_Z = -844;
GLfloat c_angel = 0;

GLfloat *current_student_X = &student_a_X;
GLfloat *current_student_Z = &student_a_Z;
GLfloat *current_angel = &a_angel;

int studentMode = student_a;
int spotlightMode = yes;
void rectangle(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d, int mode) {
	//glColor3f(0, 0, 1);
	//glBegin(GL_LINE_LOOP);
	glBegin(GL_POLYGON);
	if (mode == front) glNormal3f(0, 0, 1);
	if (mode == back) glNormal3f(0, 0, -1);
	if (mode == left) glNormal3f(-1, 0, 0);
	if (mode == right) glNormal3f(1, 0, 0);
	if (mode == top) glNormal3f(0, 1, 0);
	if (mode == bottom) glNormal3f(0, -1, 0);
	glVertex3fv(d);
	glVertex3fv(c);
	glVertex3fv(b);
	glVertex3fv(a);
	glEnd();
}
void divide_rectangle(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d, int m, int mode) {
	GLfloat p1[3], p2[3], p3[3], p4[3], p5[3];
	if (m > 0) {
		for (int i = 0; i < 3; i++) {
			p1[i] = (a[i] + b[i]) / 2;
			p2[i] = (b[i] + c[i]) / 2;
			p3[i] = (c[i] + d[i]) / 2;
			p4[i] = (d[i] + a[i]) / 2;
			p5[i] = (p1[i] + p3[i]) / 2;
		}
		divide_rectangle(a, p1, p5, p4, m - 1, mode);
		divide_rectangle(p1, b, p2, p5, m - 1, mode);
		divide_rectangle(p5, p2, c, p3, m - 1, mode);
		divide_rectangle(p4, p5, p3, d, m - 1, mode);
	}
	else {
		rectangle(a, b, c, d, mode);
	}
}

void cube(int m) {
	divide_rectangle(vertices[7], vertices[6], vertices[5], vertices[4], m, front);//front 
	divide_rectangle(vertices[2], vertices[3], vertices[0], vertices[1], m, back);//back 
	divide_rectangle(vertices[3], vertices[7], vertices[4], vertices[0], m, left);//left
	divide_rectangle(vertices[6], vertices[2], vertices[1], vertices[5], m, right);//right
	divide_rectangle(vertices[3], vertices[2], vertices[6], vertices[7], m, top);//top
	divide_rectangle(vertices[4], vertices[5], vertices[1], vertices[0], m, bottom);//bottom
}

void window() {//window model
	glScalef(7, 12, 2);
	glScalef(0.25, 0.25, 0.25);
	cube(4);
}

void lawn() {//lawn model
	glScalef(55, 2, 25);
	glScalef(0.25, 0.25, 0.25);
	cube(4);
}

void wheel(void) {
	glColor3f(0.612, 0.475, 0);
	glScalef(0.5, 0.5, 0.5);
	glBegin(GL_POLYGON);
	glVertex3f(-5, 0, 0);
	glVertex3f(-3.5, 3.5, 0);
	glVertex3f(0, 5, 0);
	glVertex3f(3.5, 3.5, 0);
	glVertex3f(5, 0, 0);
	glVertex3f(3.5, -3.5, 0);
	glVertex3f(0, -5, 0);
	glVertex3f(-3.5, -3.5, 0);
	glEnd();

}
void car(void) {
	GLfloat mat_specular5[] = { 0.2, 0.529, 0.902, 1.0 };//set matrial
	GLfloat mat_ambient5[] = { 0.2, 0.529, 0.902, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular5);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient5);
	glPushMatrix();//body
	glScalef(25, 9, 8);
	glScalef(0.25, 0.25, 0.25);
	cube(2);
	glPopMatrix();


	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(-8, -4, 3);
	wheel();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8, -4, 3);
	wheel();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-8, -4, -3);
	wheel();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8, -4, -3);
	wheel();
	glPopMatrix();

	glPushMatrix();//window 
	glColor3f(0, 0, 0);
	glTranslatef(-9, 2, 3);
	glScalef(4, 3, 3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();//window 
	glColor3f(0, 0, 0);
	glTranslatef(-9, 2, -3);
	glScalef(4, 3, 3);
	glutSolidCube(1);
	glPopMatrix();
}

void buildingModel(void) {
	//glutSolidCube(60);
	glScalef(15, 15, 15);
	cube(numberOfPolygon);
}
void defineCamera() {
	gluLookAt(viewX, viewY, viewZ, viewX + cameraZ[0], viewY + cameraZ[1], viewZ + cameraZ[2], cameraY[0], cameraY[1], cameraY[2]);
}
void envirment(void) {//draw envirment
	glLoadIdentity();
	defineCamera();
	glTranslatef(*current_student_X, 70, *current_student_Z);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_postion);//set light 0 location

	glLoadIdentity();
	defineCamera();
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_postion);//set light 1 location

	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	
	glLoadIdentity();
	defineCamera();
	glTranslatef(viewX, viewY, viewZ);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 10.0);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 2.0);
	glLightfv(GL_LIGHT3, GL_POSITION, light3_position);//set light 3 location
	GLfloat light3_direction[] = { cameraZ[0], cameraZ[1], cameraZ[2] };
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_direction);//set light3 direction

	glEnable(GL_LIGHTING); /* enable lighting */
	//glEnable(GL_LIGHT3);
	if (lightMode == 0) {
		glEnable(GL_LIGHT0);
	}
	if (lightMode == 1) {
		glEnable(GL_LIGHT1);
	}
	if (lightMode == 2) {
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}
	glEnable(GL_LIGHT2);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);

	GLfloat mat_specular[] = { 0.843, 0.365, 0.075, 1.0 };//set material
	GLfloat mat_ambient[] = { 0.843, 0.365, 0.075, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

	glLoadIdentity();
	defineCamera();
	//glColor3f(0.843, 0.365, 0.075);
	glTranslatef(100, 30, -800);
	glScalef(1.5, 1, 0.8);
	buildingModel();

	glLoadIdentity();
	//glColor3f(0.843, 0.365, 0.075);
	defineCamera();
	glTranslatef(280, 30, -800);
	glScalef(1.5, 1, 0.8);
	buildingModel();

	glLoadIdentity();
	//glColor3f(0.843, 0.365, 0.075);
	defineCamera();
	glTranslatef(190, 30, -800);
	glScalef(1, 1, 1);
	buildingModel();

	glLoadIdentity();
	//glColor3f(0.843, 0.365, 0.075);
	defineCamera();
	glTranslatef(70, 30, -780);
	glScalef(0.5, 1, 1.2);
	buildingModel();

	glLoadIdentity();
	//glColor3f(0.843, 0.365, 0.075);
	defineCamera();
	glTranslatef(310, 30, -780);
	glScalef(0.5, 1, 1.2);
	buildingModel();

	GLfloat mat_specular1[] = { 0.6, 0.6, 0.6, 1.0 };//set material for building
	GLfloat mat_ambient1[] = { 0.6, 0.6, 0.6, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular1);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient1);
	glLoadIdentity();
	//glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(100, 30, -700);
	glScalef(1.5, 1, 0.7);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(280, 30, -700);
	glScalef(1.5, 1, 0.7);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(320, 30, -630);
	glScalef(1.5, 1, 0.7);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(314, 30, -600);
	glScalef(1.5, 1, 0.7);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(500, 30, -820);
	glScalef(2.5, 1, 0.6);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(460, 30, -770);
	glScalef(1, 1, 0.4);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(460, 30, -730);
	glScalef(1, 1, 0.4);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(460, 30, -690);
	glScalef(1, 1, 0.4);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(480, 30, -650);
	glScalef(1.8, 1, 0.4);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(560, 30, -700);
	glScalef(0.4, 1, 1.8);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(680, 30, -740);
	glScalef(1.7, 1, 1.6);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(680, 30, -836);
	glScalef(1.7, 1, 0.6);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(760, 30, -650);
	glScalef(1.5, 1, 0.7);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(820, 30, -850);
	glScalef(1.5, 1, 0.7);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(850, 30, -790);
	buildingModel();

	glLoadIdentity();
	glColor3f(0.6, 0.6, 0.6);
	defineCamera();
	glTranslatef(859, 30, -750);
	glScalef(0.7, 1, 1.5);
	buildingModel();

	GLfloat mat_specular2[] = { 0.627, 0.627, 0.627, 1.0 };//set material
	GLfloat mat_ambient2[] = { 0.627, 0.627, 0.627, 1.0 };//ground
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular2);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient2);
	glLoadIdentity();
	defineCamera();
	glTranslatef(500, -2, -500);
	glScalef(250, 1, 250);
	cube(6);
	/*******************************************************************************/
	GLfloat mat_specular3[] = { 0, 1, 0, 1.0 };//set material for lawn
	GLfloat mat_ambient3[] = { 0, 1, 0, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular3);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient3);
	glLoadIdentity();//lawn
	defineCamera();
	glTranslatef(116, 0, -746);
	lawn();
	glLoadIdentity();//lawn
	defineCamera();
	glTranslatef(264, 0, -746);
	lawn();

	GLfloat mat_specular4[] = { 0.216, 0.373, 0.522, 1.0 };//set material
	GLfloat mat_ambient4[] = { 0.216, 0.373, 0.522, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular4);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient4);
	glLoadIdentity();//window at back
	defineCamera();
	glTranslatef(170, 12, -770.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(170, 30, -770.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(170, 48, -770.5);
	window();

	glLoadIdentity();
	defineCamera();
	glTranslatef(180, 12, -770.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(180, 30, -770.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(180, 48, -770.5);
	window();

	glLoadIdentity();
	defineCamera();
	glTranslatef(211, 12, -770.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(211, 30, -770.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(211, 48, -770.5);
	window();

	glLoadIdentity();
	defineCamera();
	glTranslatef(200, 12, -770.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(200, 30, -770.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(200, 48, -770.5);
	window();

	glLoadIdentity();//window at front
	defineCamera();
	glTranslatef(170, 12, -830.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(170, 30, -830.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(170, 48, -830.5);
	window();

	glLoadIdentity();
	defineCamera();
	glTranslatef(180, 12, -830.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(180, 30, -830.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(180, 48, -830.5);
	window();

	glLoadIdentity();
	defineCamera();
	glTranslatef(211, 12, -830.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(211, 30, -830.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(211, 48, -830.5);
	window();

	glLoadIdentity();
	defineCamera();
	glTranslatef(200, 12, -830.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(200, 30, -830.5);
	window();
	glLoadIdentity();
	defineCamera();
	glTranslatef(200, 48, -830.5);
	window();//end
/***********************************************/
	glLoadIdentity();//van
	defineCamera();
	glTranslatef(420, 7, -600);
	glRotatef(-45, 0, 1, 0);
	car();
	glDisable(GL_LIGHTING);
}
void myinit(void) {
	glEnable(GL_LIGHT3);
	glClearColor(0.427, 0.847, 0.984, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 1, 0.5, 10000);
	glMatrixMode(GL_MODELVIEW);
}
void DrawFacuty(void) {
	glPushMatrix();//draw body
	glColor3f(0.427, 0.561, 0.984);
	glScalef(1, 1.5, 1);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw hair
	glColor3f(0, 0, 0);
	glTranslatef(0, 5.2, 0);
	glScalef(0.5, 0.1, 0.5);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw head
	glColor3f(1, 0.804, 0.38);
	glTranslatef(0, 4, 0);
	glScalef(0.5, 0.5, 0.5);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw ear
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-1.2, 4.1, 0);
	glScalef(0.1, 0.2, 0.2);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw ear
	glColor3f(1, 0.804, 0.38);
	glTranslatef(1.2, 4.1, 0);
	glScalef(0.1, 0.2, 0.2);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw eye
	glColor3f(0, 0, 0);
	glTranslatef(-0.5, 4.3, 1.1);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw eye
	glColor3f(0, 0, 0);
	glTranslatef(0.5, 4.3, 1.1);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw nose
	glColor3f(1, 1, 0);
	glTranslatef(0, 4, 1.1);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw mouth
	glColor3f(1, 0.847, 0.878);
	glTranslatef(0, 3.5, 1.1);
	glScalef(2, 1, 1);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger1
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-5, -1.8, 0.5);
	glScalef(1, 2, 1);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw hand
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-5, -1.3, 0);
	glRotatef(-37, 0, 0, 1);
	glScalef(1, 2, 3.5);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger2
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-5.4, -1.8, 0.5);
	glRotatef(-30, 0, 0, 1);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger3
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-5.4, -1.8, 0.2);
	glRotatef(-30, 0, 0, 1);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger4
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-5.4, -1.8, -0.1);
	glRotatef(-30, 0, 0, 1);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger5
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-5.4, -1.8, -0.4);
	glRotatef(-30, 0, 0, 1);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger1
	glColor3f(1, 0.804, 0.38);
	glTranslatef(2.5, -2.6, 0.5);
	glScalef(1, 2, 1);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw hand
	glColor3f(1, 0.804, 0.38);
	glTranslatef(2.7, -2.2, 0);
	glScalef(1, 2, 3.5);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger2
	glColor3f(1, 0.804, 0.38);
	glTranslatef(2.7, -2.7, 0.4);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger3
	glColor3f(1, 0.804, 0.38);
	glTranslatef(2.7, -2.7, 0.1);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger4
	glColor3f(1, 0.804, 0.38);
	glTranslatef(2.7, -2.7, -0.2);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger5
	glColor3f(1, 0.804, 0.38);
	glTranslatef(2.7, -2.7, -0.5);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw arm
	glColor3f(0.427, 0.561, 0.984);
	glTranslatef(-3.5, 1, 0);
	glRotatef(-37, 0, 0, 1);
	glScalef(0.3, 1.2, 0.3);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw arm
	glColor3f(0.427, 0.561, 0.984);
	glTranslatef(2.5, 0.5, 0);
	glScalef(0.3, 1.2, 0.3);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw leg
	glColor3f(0.455, 0.125, 0.682);
	glTranslatef(-1.3, -6, 0);
	glRotatef(-5, 0, 0, 1);
	glScalef(0.4, 1.7, 0.4);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw leg
	glColor3f(0.455, 0.125, 0.682);
	glTranslatef(1.3, -6, 0);
	glRotatef(5, 0, 0, 1);
	glScalef(0.4, 1.7, 0.4);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw foot
	glColor3f(0, 0, 1);
	glTranslatef(-1.5, -9.8, 0.5);
	glRotatef(-5, 0, 0, 1);
	glScalef(0.4, 0.2, 0.6);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw foot
	glColor3f(0, 0, 1);
	glTranslatef(1.5, -9.8, 0.5);
	glRotatef(5, 0, 0, 1);
	glScalef(0.4, 0.2, 0.6);
	glutSolidCube(4);
	glPopMatrix();
}
void DrawStudent(void) {//draw student 
	glPushMatrix();//draw body
	glColor3f(0, 0, 1);
	glScalef(1, 1.5, 1);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw hair
	glColor3f(0, 0, 0);
	glTranslatef(0, 5.2, 0);
	glScalef(0.5, 0.1, 0.5);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw head
	glColor3f(1, 0.804, 0.38);
	glTranslatef(0, 4, 0);
	glScalef(0.5, 0.5, 0.5);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw ear
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-1.2, 4.1, 0);
	glScalef(0.1, 0.2, 0.2);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw ear
	glColor3f(1, 0.804, 0.38);
	glTranslatef(1.2, 4.1, 0);
	glScalef(0.1, 0.2, 0.2);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw eye
	glColor3f(0, 0, 0);
	glTranslatef(-0.5, 4.3, 1.1);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw eye
	glColor3f(0, 0, 0);
	glTranslatef(0.5, 4.3, 1.1);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw nose
	glColor3f(1, 1, 0);
	glTranslatef(0, 4, 1.1);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw mouth
	glColor3f(1, 0.847, 0.878);
	glTranslatef(0, 3.5, 1.1);
	glScalef(2, 1, 1);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger1
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-5, -1.8, 0.5);
	glScalef(1, 2, 1);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw hand
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-5, -1.3, 0);
	glRotatef(-37, 0, 0, 1);
	glScalef(1, 2, 3.5);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger2
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-5.4, -1.8, 0.5);
	glRotatef(-30, 0, 0, 1);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger3
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-5.4, -1.8, 0.2);
	glRotatef(-30, 0, 0, 1);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger4
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-5.4, -1.8, -0.1);
	glRotatef(-30, 0, 0, 1);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger5
	glColor3f(1, 0.804, 0.38);
	glTranslatef(-5.4, -1.8, -0.4);
	glRotatef(-30, 0, 0, 1);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger1
	glColor3f(1, 0.804, 0.38);
	glTranslatef(2.5, -2.6, 0.5);
	glScalef(1, 2, 1);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw hand
	glColor3f(1, 0.804, 0.38);
	glTranslatef(2.7, -2.2, 0);
	glScalef(1, 2, 3.5);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger2
	glColor3f(1, 0.804, 0.38);
	glTranslatef(2.7, -2.7, 0.4);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger3
	glColor3f(1, 0.804, 0.38);
	glTranslatef(2.7, -2.7, 0.1);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger4
	glColor3f(1, 0.804, 0.38);
	glTranslatef(2.7, -2.7, -0.2);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw finger4
	glColor3f(1, 0.804, 0.38);
	glTranslatef(2.7, -2.7, -0.5);
	glScalef(1, 2.2, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();//draw arm
	glColor3f(0, 0, 1);
	glTranslatef(-3.5, 1, 0);
	glRotatef(-37, 0, 0, 1);
	glScalef(0.3, 1.2, 0.3);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw arm
	glColor3f(0, 0, 1);
	glTranslatef(2.5, 0.5, 0);
	glScalef(0.3, 1.2, 0.3);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw leg
	glTranslatef(-1.3, -6, 0);
	glRotatef(-5, 0, 0, 1);
	glScalef(0.4, 1.7, 0.4);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw leg
	glTranslatef(1.3, -6, 0);
	glRotatef(5, 0, 0, 1);
	glScalef(0.4, 1.7, 0.4);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw books
	glColor3f(1, 0, 0);
	glTranslatef(-2.5, -0.5, 0);
	glScalef(0.2, 0.6, 0.8);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw books
	glColor3f(0, 1, 0);
	glTranslatef(-3.1, -0.7, 0);
	glScalef(0.1, 0.4, 0.8);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw bag
	glColor3f(1, 0, 0);
	glTranslatef(0, 1.7, -2.9);
	glScalef(0.7, 0.7, 0.4);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw foot
	glColor3f(1, 0, 1);
	glTranslatef(-1.5, -9.8, 0.5);
	glRotatef(-5, 0, 0, 1);
	glScalef(0.4, 0.2, 0.6);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();//draw foot
	glTranslatef(1.5, -9.8, 0.5);
	glRotatef(5, 0, 0, 1);
	glScalef(0.4, 0.2, 0.6);
	glutSolidCube(4);
	glPopMatrix();
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*glLoadIdentity();
	defineCamera();
	glTranslatef(180, 70, -750);
	glutSolidCube(5);*/
	glLoadIdentity();
	envirment();

	glLoadIdentity();//student_a
	defineCamera();
	glTranslatef(student_a_X, 5, student_a_Z);
	glRotatef(a_angel, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	DrawStudent();
	
	glLoadIdentity();//student_b
	defineCamera();
	glTranslatef(student_b_X, 5, student_b_Z);
	glRotatef(b_angel, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	DrawStudent();

	glLoadIdentity();//student_c
	defineCamera();
	glTranslatef(student_c_X, 5, student_c_Z);
	glRotatef(c_angel, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	DrawStudent();

	glLoadIdentity();//student
	defineCamera();
	glTranslatef(210, 5, -844);
	glRotatef(90, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	DrawStudent();

	glLoadIdentity();//student
	defineCamera();
	glTranslatef(342, 5, -740);
	glScalef(0.5, 0.5, 0.5);
	DrawStudent();

	glLoadIdentity();//student
	defineCamera();
	glTranslatef(342, 5, -720);
	glScalef(0.5, 0.5, 0.5);
	DrawStudent();

	glLoadIdentity();//student
	defineCamera();
	glTranslatef(342, 5, -700);
	glScalef(0.5, 0.5, 0.5);
	DrawStudent();

	glLoadIdentity();//student
	defineCamera();
	glTranslatef(398, 5, -787);
	glRotatef(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	DrawStudent();

	glLoadIdentity();//student
	defineCamera();
	glTranslatef(398, 5, -793);
	glRotatef(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	DrawStudent();

	glLoadIdentity();//facuty
	defineCamera();
	glTranslatef(342, 5, -785);
	glRotatef(20, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	DrawFacuty();

	glLoadIdentity();//facuty
	defineCamera();
	glTranslatef(398, 5, -800);
	glRotatef(90, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	DrawFacuty();

	glLoadIdentity();//facuty
	defineCamera();
	glTranslatef(480, 5, -860);
	glRotatef(-90, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	DrawFacuty();

	glLoadIdentity();//facuty
	defineCamera();
	glTranslatef(540, 5, -852);
	glRotatef(-90, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	DrawFacuty();

	glFlush();
	glutSwapBuffers();
}

void cameraPath() {

}
void left_right(int angel) {
	GLfloat T[16];
	glPushMatrix();
	glLoadIdentity();
	glRotatef(angel, 0, 1, 0);//always rotate around Y axis 
	glMultMatrixf(cameraZ);//calculate the new coordinate of camera's faceing direction 
	glGetFloatv(GL_MODELVIEW_MATRIX, T);//retrieve result to array T
	cameraZ[0] = T[0];//copy result to cameraZ
	cameraZ[1] = T[1];
	cameraZ[2] = T[2];

	glLoadIdentity();
	glRotatef(angel, 0, 1, 0);
	glMultMatrixf(cameraX);
	glGetFloatv(GL_MODELVIEW_MATRIX, T);
	cameraX[0] = T[0];
	cameraX[1] = T[1];
	cameraX[2] = T[2];

	glLoadIdentity();
	glRotatef(angel, 0, 1, 0);
	glMultMatrixf(cameraY);
	glGetFloatv(GL_MODELVIEW_MATRIX, T);
	glPopMatrix();
	cameraY[0] = T[0];
	cameraY[1] = T[1];
	cameraY[2] = T[2];
}

void up_down(int angel) {
	GLfloat T[16];
	glPushMatrix();
	glLoadIdentity();
	glRotatef(angel, cameraX[0], cameraX[1], cameraX[2]);// rotate around the X axis of camera in order to look up
	glMultMatrixf(cameraZ);
	glGetFloatv(GL_MODELVIEW_MATRIX, T);
	cameraZ[0] = T[0];
	cameraZ[1] = T[1];
	cameraZ[2] = T[2];

	glLoadIdentity();
	glRotatef(angel, cameraX[0], cameraX[1], cameraX[2]);
	glMultMatrixf(cameraY);
	glGetFloatv(GL_MODELVIEW_MATRIX, T);
	glPopMatrix();
	cameraY[0] = T[0];
	cameraY[1] = T[1];
	cameraY[2] = T[2];
}
void translation(int m, int v) {
	if (m == front) {
		viewX = viewX + v * cameraZ[0];
		viewY = viewY + v * cameraZ[1];
		viewZ = viewZ + v * cameraZ[2];
	}
	if (m == back) {
		viewX = viewX - v * cameraZ[0];
		viewY = viewY - v * cameraZ[1];
		viewZ = viewZ - v * cameraZ[2];
	}
	if (m == right) {
		viewX = viewX + v * cameraX[0];
		viewY = viewY + v * cameraX[1];
		viewZ = viewZ + v * cameraX[2];
	}
	if (m == left) {
		viewX = viewX - v * cameraX[0];
		viewY = viewY - v * cameraX[1];
		viewZ = viewZ - v * cameraX[2];
	}
}
int collisionDetect(float px, float py, float pz) {
	int collision = no;
	int iterater = 0;
	float adjustX = 0;
	float adjustZ = 0;
	float x=0, z=0;
	//detect building collison
	while (iterater < 360) {
		adjustX = cos(iterater * PI / 180) * 2.8;
		adjustZ = sin(iterater * PI / 180) * 2.8;
		x = px + adjustX;
		z = pz + adjustZ;
		if (x > 56 && x <= 87 && py < 60) {
			if (z > -824 && z < -744 || z > -720 && z < -680) return collision = yes;
		}
		else if (x > 87 && x <= 145 && py < 60) {
			if (z > -824 && z < -774 || z > -720 && z < -680)  return collision = yes;
		}
		else if ( x > 162 && x <= 222 && py < 60) {
			if (z > -827 && z < -768) return collision = yes;
		}
		else if (x > 237 && x <= 270 && py < 60) {
			if (z > -824 && z < -774 || z > -720 && z < -680) return collision = yes;
		}
		else if (x > 271 && x <= 277 && py < 60) {
			if (z > -824 && z < -774 || z > -720 && z < -680 || z > -618 && z < -578)return collision = yes;
		}
		else if (x > 277 && x <= 295 && py < 60) {
			if (z > -824 && z < -774 || z > -720 && z < -680 || z > -648 && z < -578)return collision = yes;
		}
		else if (x > 295 && x <= 328 && py < 60) {
			if (z > -824 && z < -744 || z > -720 && z < -680 || z > -648 && z < -578)return collision = yes;
		}
		else if (x > 328 && x <= 360 && py < 60) {
			if (z > -648 && z < -578) return collision = yes;
		}
		else if (x > 360 && x <= 367 && py < 60) {
			if (z > -648 && z < -608) return collision = yes;
		}
		else if (x > 426 && x <= 432 && py < 60) {
			if (z > -837 && z < -802 || z > -661 && z < -638) return collision = yes;
		}
		else if (x > 432 && x <= 492 && py < 60) {
			if (z > -837 && z < -802 || z > -780 && z< -757 || z > -740 && z < -717 || z > -700 && z < -677 ||z > -661 && z < -638)return collision = yes;
		}
		else if (x > 492 && x <= 536 && py < 60) {
			if (z > -837 && z < -802 || z > -661 && z < -638) return collision = yes;
		}
		else if (x > 536 && x <= 550 && py < 60) {
			if (z > -837 && z < -802) return collision = yes;
		}
		else if (x > 550 && x <= 573 && py < 60) {
			if (z > -837 && z < -802 || z > -752 && z < -645) return collision = yes;
		}
		else if (x > 573 && x <= 577 && py < 60) {
			if (z > -837 && z < -802) return collision = yes;
		}
		else if (x > 631 && x <= 716 && py < 60) {
			if (z > -852 && z < -817 || z > -786 && z < -691) return collision = yes;
		}
		else if (x > 716 && x <= 732 && py < 60) {
			if (z > -852 && z < -817 || z > -786 && z < -691 || z > -669 && z < -629) return collision = yes;
		}
		else if (x > 732 && x <= 776 && py < 60) {
			if (z > -669 && z < -629) return collision = yes;
		}
		else if (x > 776 && x <= 805 && py < 60) {
			if (z > -869 && z < -827 || z > -669 && z < -629)  return collision = yes;
		}
		else if (x > 805 && x <= 821 && py < 60) {
			if (z > -869 && z < -827) return collision = yes;
		}
		else if (x > 821 && x <= 840 && py < 60) {
			if (z > -869 && z < -827 || z > -818 && z < -760) return collision = yes;
		}
		else if (x > 840 && x <= 865 && py < 60) {
			if (z > -869 && z < -827 || z > -818 && z < -704) return collision = yes;
		}
		else if (x > 865 && x <= 880 && py < 60) {
			if(z > -818 && z < -704)return collision = yes;
		}
		iterater += 5;
	}
	//detect people collision
	GLfloat distance = (student_a_X - px)*(student_a_X - px) + (student_a_Z - pz)*(student_a_Z - pz);
	if ( distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	distance = (student_b_X - px)*(student_b_X - px) + (student_b_Z - pz)*(student_b_Z - pz);
	if (distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	distance = (student_c_X - px)*(student_c_X - px) + (student_c_Z - pz)*(student_c_Z - pz);
	if (distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	distance = (210 - px)*(210 - px) + (-844 - pz)*(-844 - pz);
	if (distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	distance = (342 - px)*(342 - px) + (-740 - pz)*(-740 - pz);
	if (distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	distance = (342 - px)*(342 - px) + (-720 - pz)*(-720 - pz);
	if (distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	distance = (342 - px)*(342 - px) + (-700 - pz)*(-700 - pz);
	if (distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	distance = (398 - px)*(398 - px) + (-787 - pz)*(-787 - pz);
	if (distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	distance = (398 - px)*(398 - px) + (-793 - pz)*(-793 - pz);
	if (distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	distance = (342 - px)*(342 - px) + (-785 - pz)*(-785 - pz);
	if (distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	distance = (398 - px)*(398 - px) + (-800 - pz)*(-800 - pz);
	if (distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	distance = (480 - px)*(480 - px) + (-860 - pz)*(-860 - pz);
	if (distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	distance = (540 - px)*(540 - px) + (-852 - pz)*(-852 - pz);
	if (distance < 30.25 && distance != 0 && py < 8) return collision = yes;
	//van detection
	distance = (416 - px)*(416 - px) + (-603.8 - pz)*(-603.8 - pz);
	if (distance < 46.24 && py < 12) return collision = yes;
	distance = (424 - px)*(424 - px) + (-596.2 - pz)*(-596.2 - pz);
	if (distance < 46.24 && py < 12) return collision = yes;
	//ground detection
	if(py < 0) return collision = yes;
	//camera detection
	distance = (viewX - px)*(viewX - px) + (viewZ - pz)*(viewZ - pz) + (viewY - py)*(viewY - py);
	if (distance < 64 && distance != 0) collision = yes;
	return collision;
}
void myKeyboard(unsigned char key, int x, int y) {
	GLfloat T[16];
	if (key == 'a') {//rotate left
		left_right(5);
		glutPostRedisplay();
	}
	if (key == 'd') {//rotate right
		left_right(-5);
		glutPostRedisplay();
	}
	if (key == 'w') {//look up
		up_down(5);
		glutPostRedisplay();
	}
	if (key == 's') {//look down
		up_down(-5);
		glutPostRedisplay();
	}
	GLfloat view_x, view_y, view_z;
	if (key == 'h') {//move to the left
		view_x = viewX;
		view_y = viewY;
		view_z = viewZ;
		translation(left, 10);
		if (collisionDetect(viewX, viewY, viewZ) == no)glutPostRedisplay();
		else {
			viewX = view_x;
			viewY = view_y;
			viewZ = view_z;
			glutPostRedisplay();
		}
	}
	if (key == 'k') {//move to the right
		view_x = viewX;
		view_y = viewY;
		view_z = viewZ;
		translation(right, 10);
		if (collisionDetect(viewX, viewY, viewZ) == no)glutPostRedisplay();
		else {
			viewX = view_x;
			viewY = view_y;
			viewZ = view_z;
			glutPostRedisplay();
		}
	}
	if (key == 'u') {//move the direction you face
		view_x = viewX;
		view_y = viewY;
		view_z = viewZ;
	    translation(front, 10);
		if (collisionDetect(viewX, viewY, viewZ) == no) glutPostRedisplay();
		else {
			viewX = view_x;
			viewY = view_y;
			viewZ = view_z;
			glutPostRedisplay();
		}
	}
	if (key == 'j') {//move the oppsite direction you face
		view_x = viewX;
		view_y = viewY;
		view_z = viewZ;
		translation(back, 10);
		if (collisionDetect(viewX, viewY, viewZ) == no) glutPostRedisplay();
		else {
			viewX = view_x;
			viewY = view_y;
			viewZ = view_z;
			glutPostRedisplay();
		}
	}

	if (key == 'l') {
		lightMode++;
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		if (lightMode == 4) lightMode = 0;
		glutPostRedisplay();
	}
	if (key == '=') {
		intensityControl = intensityControl + 0.1;
		if (intensityControl > 1) intensityControl = 1;
		light0_diffuse[0] = intensityControl;
		light0_diffuse[1] = intensityControl;
		light0_diffuse[2] = intensityControl;
		light1_diffuse[0] = 0.8 * intensityControl;
		light1_diffuse[1] = 0.2 * intensityControl;
		light1_diffuse[2] = intensityControl;
		glutPostRedisplay();
	}
	if (key == '-') {
		intensityControl = intensityControl - 0.1;
		if (intensityControl < 0) intensityControl = 0;
		light0_diffuse[0] = intensityControl;
		light0_diffuse[1] = intensityControl;
		light0_diffuse[2] = intensityControl;
		light1_diffuse[0] = 0.8 * intensityControl;
		light1_diffuse[1] = 0.2 * intensityControl;
		light1_diffuse[2] = intensityControl;
		glutPostRedisplay();
	}
	if (key == '0') {
		numberOfPolygon = 0;
		glutPostRedisplay();
	}
	if (key == '1') {
		numberOfPolygon = 1;
		glutPostRedisplay();
	}
	if (key == '2') {
		numberOfPolygon = 2;
		glutPostRedisplay();
	}
	if (key == '3') {
		numberOfPolygon = 3;
		glutPostRedisplay();
	}
	if (key == '4') {
		numberOfPolygon = 4;
		glutPostRedisplay();
	}
	if (key == 'f') {
		cameraX[0] = 1;//rest camera
		cameraX[1] = 0;
		cameraX[2] = 0;
		cameraY[0] = 0;
		cameraY[1] = 0.6;
		cameraY[2] = -0.8;
		cameraZ[0] = 0;
		cameraZ[1] = -0.8;
		cameraZ[2] = -0.6;
		viewX = 500;
		viewY = 900;
		viewZ = 0;
	    //define the path of auto camera
		while (viewX > 187) {
			translation(left, 5);
			display();
		}
		while (viewY > 30) {
			translation(front, 10);
			display();
		}
		while (cameraY[2] < 0) {
			up_down(2);
			display();
		}
		while (viewZ > -733){
			translation(front, 2);
			display();
		}
		while (cameraZ[2] < 0) {
			left_right(-2);
			display();
		}
		while (viewX < 374) {
			translation(front, 2);
			display();
		}
		while (cameraY[0] < 0.4) {
			up_down(-2);
			display();
		}
		for (int x = 0; x < 450; x++) {
			left_right(1);
			display();
		}
		while (cameraY[2] < 0) {
			up_down(2);
			display();
		}
		while (viewZ > -987) {
			translation(front, 2);
			display();
		}
		for (int x = 0; x < 130; x++) {
			left_right(1);
			display();
		}
	}
	if (key == 'g') {
		studentMode++;
		if (studentMode == 4) studentMode = 1;
		if (studentMode == student_a) {
			current_student_X = &student_a_X;
			current_student_Z = &student_a_Z;
			current_angel = &a_angel;
		}
		if (studentMode == student_b) {
			current_student_X = &student_b_X;
			current_student_Z = &student_b_Z;
			current_angel = &b_angel;
		}
		if (studentMode == student_c) {
			current_student_X = &student_c_X;
			current_student_Z = &student_c_Z;
			current_angel = &c_angel;
		}
		glutPostRedisplay();
	}
	if (key == 'v') {
		if (spotlightMode == yes) {
			spotlightMode = no;
			glDisable(GL_LIGHT3);
		}
		else {
			spotlightMode = yes;
			glEnable(GL_LIGHT3);
		}
		glutPostRedisplay();
	}
}

void mySpecial(int key, int x, int y) {
	GLfloat X = sin((*current_angel) * PI / 180) * 5;
	GLfloat Z = cos((*current_angel) * PI / 180) * 5;
	if (key == GLUT_KEY_UP) {//student move forward
		(*current_student_X) = (*current_student_X) + X;
		(*current_student_Z) = (*current_student_Z) + Z;
		if (collisionDetect(*current_student_X, 0, *current_student_Z) == no) {
			glutPostRedisplay();
		}
		else {
			(*current_student_X) = (*current_student_X) - X;
			(*current_student_Z) = (*current_student_Z) - Z;
			glutPostRedisplay();
		}
	}
	if (key == GLUT_KEY_DOWN) {//student move backward
		(*current_student_X) = (*current_student_X) - X;
		(*current_student_Z) = (*current_student_Z) - Z;
		if (collisionDetect(*current_student_X, 0, *current_student_Z) == no) {
			glutPostRedisplay();
		}
		else {
			(*current_student_X) = (*current_student_X) + X;
			(*current_student_Z) = (*current_student_Z) + Z;
			glutPostRedisplay();
		}
	}
	if (key == GLUT_KEY_LEFT) {//student rotate counter clockwise
		(*current_angel) = (*current_angel) + 5;
		if ((*current_angel) > 360) (*current_angel) -= 360;
		glutPostRedisplay();
	}
	if (key == GLUT_KEY_RIGHT) {//student rotate clockwise
		(*current_angel) = (*current_angel) - 5;
		if ((*current_angel) < 0) (*current_angel) += 360;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("COMP-5/6400 Assignment 6");
	myinit();
	glEnable(GL_DEPTH_TEST);//enable depth test
	glutDisplayFunc(display);

	glutSpecialFunc(mySpecial);
	glutKeyboardFunc(myKeyboard);

	glutMainLoop();
	return 0;
}