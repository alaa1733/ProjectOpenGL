/* 
-- MAZE PROJECT made by ALAA MOHAMMED --
-- The idea is the Butterfly will not arrive at the flower until you answer the question --
 -- if you answer wrong the light will be decreased if true the light will rise --
 */

#include <cmath>
#include <stdlib.h>
#include <GL\freeglut.h> 
#include <stdio.h>
#include <FreeImage.h>  
#include <iostream>

using namespace std;

#define PI 3.1415926535898
#define Cos(th) cos(PI/180*(th))
#define Sin(th) sin(PI/180*(th))


int th = -41;
int ph = 139;
int fofview = 50;
float r = 5;

GLint win_width = 500,
win_hight = 500;

float maze_width = 0.02;

float lightx = -23.3001;
float lighty = -23.1001;
float lightz = 31.5001;

//from 0 to 1 where 1.5 is at full strength
float light_strength = 1.1;

//////////////////////////////////////////////////////////////////
GLuint myTexture1;
GLuint myTexture2;
GLuint myTexture3;
GLuint myTexture4;
GLuint myTexture5;
GLuint myTexture6;
GLuint myTexture7;
GLuint myTexture8;
int ss;
GLuint TT[8];
GLuint selected;
char image1Path[] = "1.bmp";
char image2Path[] = "q1.bmp";
char image3Path[] = "q2.bmp";
char image4Path[] = "q3.bmp";
char image5Path[] = "q4.bmp";
char image6Path[] = "q5.bmp";
char image7Path[] = "q6.bmp";
char image8Path[] = "end.bmp";
//////////////////////////////////////////////////////////////////

GLuint load(const char* texName) {
	GLuint tex;

	FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(texName);
	FIBITMAP* bitmap = FreeImage_Load(format, texName, 0);
	FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap);

	FreeImage_Unload(bitmap);

	void* texdata = FreeImage_GetBits(bitmap2);

	if (texdata) {
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FreeImage_GetWidth(bitmap2), FreeImage_GetHeight(bitmap2), 0, GL_BGR_EXT,
			GL_UNSIGNED_BYTE, texdata);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		return tex;
	}
	else {

		return 0;

	}

}

GLuint maze_tex;
GLuint maze_tex_dark;
GLuint backC;
GLuint cubeTex;

bool moving = false;

float curretposX = -0.75f;
float curretposY = -0.6f;
int stage = 0;

GLfloat cube1[3] = { 0.5, 0.5, 0.5 };
GLfloat cube2[3] = { -0.5, 0.5, 0.5 };
GLfloat cube3[3] = { -0.5,-0.5, 0.5 };
GLfloat cube4[3] = { 0.5,-0.5, 0.5 };
GLfloat cube5[3] = { 0.5, 0.5,-0.5 };
GLfloat cube6[3] = { -0.5, 0.5,-0.5 };
GLfloat cube7[3] = { -0.5,-0.5,-0.5 };
GLfloat cube8[3] = { 0.5,-0.5,-0.5 };






float butterfly_pos[7][2][3] = {
	{{-0.75, -0.6f,-90},{-0.2, -0.6f,-90}},
	{{-0.2, -0.6f,0},{-0.2, -0.2f,0}},
	{{-0.2, -0.2f,-90},{0.18, -0.2f,-90}},
	{{0.18, -0.2f,0},{0.18, 0.05f,0}},
	{{0.18, 0.05f,-90},{0.33, 0.05f,-90}},
	{{0.33, 0.05f,0},{0.33, 0.57f,0}},
	{{0.33, 0.57f,-90 },{0.85, 0.57f,-90}},
};


void d_cube(float x, float y, float z, float scaleX, float scaleY, float scaleZ) {
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(x, y, z);
	glScalef(scaleX, scaleY, scaleZ);

	glBegin(GL_QUADS);

	//front
	glColor3f(1.0, 1.0, 0.0);
	glVertex3fv(cube1);
	glVertex3fv(cube2);
	glVertex3fv(cube3);
	glVertex3fv(cube4);

	//back
	glColor3f(1.0, 0.0, 0.0);
	glVertex3fv(cube6);
	glVertex3fv(cube5);
	glVertex3fv(cube8);
	glVertex3fv(cube7);

	//right
	glColor3f(0.0, 1.0, 0.0);
	glVertex3fv(cube5);
	glVertex3fv(cube1);
	glVertex3fv(cube4);
	glVertex3fv(cube8);

	//left
	glColor3f(0.0, 0.0, 1.0);
	glVertex3fv(cube2);
	glVertex3fv(cube6);
	glVertex3fv(cube7);
	glVertex3fv(cube3);

	//top
	glColor3f(0.0, 1.0, 1.0);
	glVertex3fv(cube5);
	glVertex3fv(cube6);
	glVertex3fv(cube2);
	glVertex3fv(cube1);

	//buttom
	glColor3f(1.0, 0.0, 1.0);
	glVertex3fv(cube4);
	glVertex3fv(cube3);
	glVertex3fv(cube7);
	glVertex3fv(cube8);
	glEnd();
	glPopMatrix();
}

void Maze() {
	///////// Fram //////////////
	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);
	//
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.75f, 0.65f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.75f, 0.65f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.75f, -0.65f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.75f, -0.65f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.75f, 0.65f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.75f, 0.60f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.75f, -0.65f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.75f, 0.52f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.75f, -0.65f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.75f, -0.60f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.75f, 0.65f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.75f, -0.52f);
	////////// End of Fram ////////////

	//////// Iner Maze ///////////
	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.1f, 0.65f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.1f, 0.20f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.1f, 0.20f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.60f, 0.20f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.75f, -0.10f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.10f, -0.10f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.10f, -0.10f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.10f, 0.25f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.25f, -0.65f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.25f, -0.1f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.25f, -0.1f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.65f, -0.1f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.65f, -0.1f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.65f, 0.30f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.75f, 0.52f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.40f, 0.52f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.40f, 0.52f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.40f, 0.30f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.75f, -0.52f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.40f, -0.52f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.40f, -0.52f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.40f, -0.30f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.75f, -0.60f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.01f, -0.60f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.01f, -0.60f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.01f, -0.20f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.25f, 0.65f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.25f, 0.15f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.75f, -0.325f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.40f, -0.325f);

	glLineWidth(2.25f);
	glLineStipple(2, 0x00FF);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.375f, 0.20f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.375f, -0.01f);
	//////// End of iner Maze ////////////
	glEnd();

}

void back() {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	//
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1, 1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1, -1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-1, -1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, cubeTex);
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(0, 0, 0.26);
	glScalef(2, 2, 0.5);
	glBegin(GL_QUADS);

	//front
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(cube1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(cube2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(cube3);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(cube4);

	//back
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(cube6);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(cube5);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(cube8);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(cube7);

	//right
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(cube5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(cube1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(cube4);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(cube8);

	//left
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(cube2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(cube6);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(cube7);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(cube3);

	//top
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(cube5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(cube6);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(cube2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(cube1);

	//buttom
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(cube4);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(cube3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(cube7);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(cube8);
	glEnd();
	glPopMatrix();
}

void light() {

	GLfloat Ambient[] = { light_strength * 0.7, light_strength * 0.7 ,light_strength * 0.7 , 1.0 };
	GLfloat Diffuse[] = { light_strength  , light_strength , light_strength , 1.0 };
	GLfloat Specular[] = { light_strength , light_strength , light_strength , 1.0 };
	GLfloat Lposition[] = { lightx, lighty, lightz, 1.0 };

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
	glLightfv(GL_LIGHT0, GL_POSITION, Lposition);

}


void qustionWindwo() {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(1.7, 1.35);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(3.99, 1.35);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(3.99, -1.35);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(1.7, -1.35);
	glEnd();

}

void makeCircle(float Yr, float Xr, float x, float y, float red, float green, float blue, float angle = 0) {

	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(angle, 0, 0, 1);
	glColor3f(red, green, blue);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i++) {
		float th = i * 3.1415 / 180;
		glVertex2f(Xr * cos(th), Yr * sin(th));
	}

	glEnd();
	glPopMatrix();
}

void butterfly(float x, float y, float angle, float sx = 1, float sy = 1, float sz = 1) {

	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(sx, sy, sz);
	glRotatef(angle, 0, 0, 1);

	makeCircle(0.03, 0.02, -0.028, 0.015, 0.98, 0.67, 0.04, 30);
	makeCircle(0.025, 0.015, -0.013, -0.015, 0.96, 0.86, 0.02, 140);

	makeCircle(0.03, 0.02, 0.028, 0.015, 0.98, 0.67, 0.04, 30 - 80);
	makeCircle(0.025, 0.015, 0.013, -0.015, 0.96, 0.86, 0.02, 140 - 90);

	makeCircle(0.05, 0.01, 0, 0, 0.49, 0.27, 0.24);
	glPopMatrix();
}

void goTo() {
	if (stage < 7) {
		if ((fabs(curretposX - (butterfly_pos[stage][1][0])) < 0.000001) != true) {
			curretposX += 0.01f;

		}
		else if ((fabs(curretposY - (butterfly_pos[stage][1][1])) < 0.000001) != true) {
			curretposY += 0.01f;
		}
		else
		{
			moving = false;
		}
	}
}

void sun(float Yr, float Xr, float x, float y, float z, float red, float green, float blue, float angle = 0) {

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(red, green, blue);

	glBegin(GL_POLYGON);

	for (int i = 0; i <= 360; i++) {
		float th = i * 3.1415 / 180;
		glVertex2f(Xr * cos(th), Yr * sin(th));
	}

	glEnd();
	glPopMatrix();
}


void redisplay(int) {
	glutPostRedisplay();
	glutTimerFunc(20, redisplay, 0);
}

void Special(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT) {

		th += 1;
	}
	else if (key == GLUT_KEY_LEFT) {

		th -= 1;
	}
	if (key == GLUT_KEY_UP) {

		ph += 1;
	}
	else if (key == GLUT_KEY_DOWN) {

		ph -= 1;
	}

	th %= 360;
	ph %= 360;

	
	glutPostRedisplay();
}

void Keyboard(unsigned char Keyboard_value, int x, int y) {
	if (Keyboard_value == 'r') {
		selected = TT[0];
		ss = 1;

		if (moving == false) {
			stage = 0;
			curretposX = -0.75f;
			curretposY = -0.6f;
			moving = true;
		}
	}
	else if (Keyboard_value == 's') {
		selected = TT[1];
		ss = 2;
	}
	else if (ss == 2) {
		if (Keyboard_value == 'a') {
			light_strength += 0.1;
			fprintf(stderr, "\n\n right choice\n\n light strength : "
				, light_strength);
			selected = TT[ss];
			ss = 3;
			if (moving == false) {

				stage += 1;
				moving = true;
			}

		}
		else if (Keyboard_value == 'b')
		{
			light_strength -= 0.1;
			fprintf(stderr, "\n\n wrong choice\n\n light strength : "
				, light_strength);
		}
		else if (Keyboard_value == 'c')
		{
			light_strength -= 0.1;
			fprintf(stderr, "\n\n wrong choice\n\n light strength : "
				, light_strength);
		}

	}
	else if (ss == 3) {
		if (Keyboard_value == 'a') {
			light_strength -= 0.1;
			fprintf(stderr, "\n\n wrong choice\n\n light strength : "
				, light_strength);

		}
		else if (Keyboard_value == 'b')
		{
			light_strength += 0.1;
			fprintf(stderr, "\n\n right choice\n\n light strength : "
				, light_strength);

			selected = TT[ss];
			ss = 4;
			if (moving == false) {
				stage += 1;
				moving = true;
			}
		}
		else if (Keyboard_value == 'c')
		{
			light_strength -= 0.1;
			fprintf(stderr, "\n\n wrong choice\n\n light strength : "
				, light_strength);
		}

	}
	else if (ss == 4) {
		if (Keyboard_value == 'b') {
			light_strength -= 0.1;
			fprintf(stderr, "\n\n wrong choice\n\n light strength : "
				, light_strength);

		}
		else if (Keyboard_value == 'a')
		{
			light_strength += 0.1;
			fprintf(stderr, "\n\n right choice\n\n light strength : "
				, light_strength);

			selected = TT[ss];
			ss = 5;
			if (moving == false) {
				stage += 1;
				moving = true;
			}
		}
		else if (Keyboard_value == 'c')
		{
			light_strength -= 0.1;
			fprintf(stderr, "\n\n wrong choice\n\n light strength : "
				, light_strength);
		}

	}
	else if (ss == 5) {
		if (Keyboard_value == 'b') {
			light_strength -= 0.1;
			fprintf(stderr, "\n\n wrong choice\n\n light strength : "
				, light_strength);

		}
		else if (Keyboard_value == 'c')
		{
			light_strength += 0.1;
			fprintf(stderr, "\n\n right choice\n\n light strength : "
				, light_strength);

			selected = TT[ss];
			ss = 6;
			if (moving == false) {
				stage += 1;
				moving = true;
			}
		}
		else if (Keyboard_value == 'a')
		{
			light_strength -= 0.1;
			fprintf(stderr, "\n\n wrong choice\n\n light strength : "
				, light_strength);
		}

	}
	else if (ss == 6) {
		if (Keyboard_value == 'a') {
			light_strength -= 0.1;
			fprintf(stderr, "\n\n wrong choice\n\n light strength : "
				, light_strength);

		}
		else if (Keyboard_value == 'b')
		{
			light_strength += 0.1;
			fprintf(stderr, "\n\n right choice\n\n light strength : "
				, light_strength);

			selected = TT[ss];
			ss = 7;
			if (moving == false) {
				stage += 1;
				moving = true;
			}
		}
		else if (Keyboard_value == 'c')
		{
			light_strength -= 0.1;
			fprintf(stderr, "\n\n wrong choice\n\n light strength : "
				, light_strength);
		}

	}
	else if (ss == 7) {
		if (Keyboard_value == 'a') {
			light_strength -= 0.1;
			fprintf(stderr, "\n\n wrong choice\n\n light strength : "
				, light_strength);

		}
		else if (Keyboard_value == 'b')
		{
			light_strength += 0.1;
			fprintf(stderr, "\n\n right choice\n\n light strength : "
				, light_strength);

			selected = TT[ss];
			if (moving == false) {
				stage += 1;
				moving = true;
			}
		}
		else if (Keyboard_value == 'c')
		{
			light_strength -= 0.1;
			fprintf(stderr, "\n\n wrong choice\n\n light strength : "
				, light_strength);
		}

	}
	else if (Keyboard_value == 'z') {
		lightx += 0.1;

	}
	else if (Keyboard_value == 'x') {
		lightx -= 0.1;

	}

	else if (Keyboard_value == 'q') {
		exit(0);
	}
	else
	{
		fprintf(stderr, "\nKeyboard commands:\n\n"
			"q, <esc> - Quit\n");
	}

	if (Keyboard_value == '-')
	{
		r -= 0.1;
	}
	if (Keyboard_value == '+')
	{
		r += 0.1;
	}
	glutPostRedisplay();
}

void flower() {

	glTranslatef(0.15, 0.15, 0);

	float sine, cosine;

	for (int i = 0; i < 60; i++)
	{
		glColor3f(1, 0.85, 0);
		cosine = (0.19 * cos(i * 2 * 3.14159265 / 60));
		sine = (0.19 * sin(i * 2 * 3.14159265 / 60));
		glBegin(GL_POLYGON);
		glVertex2f(cosine, sine);
		glVertex2f((0.19 * cos((i + 1) * 2 * 3.14159265 / 60)), (0.19 * sin((i + 1) * 2 * 3.14159265 / 60)));

	}
	glEnd();
	//................................





}

void petals() {
	glTranslatef(0.35, 0.35, 0);

	float sine, cosine;
	glColor3f(1, 1, 1);
	for (int i = 0; i < 60; i++)
	{

		cosine = (0.3 * cos(i * 2 * 3.14159265 / 60));
		sine = (0.2 * sin(i * 2 * 3.14159265 / 60));
		glBegin(GL_POLYGON);

		glVertex2f(cosine, sine);
		glColor3f(0.57, 0, 1);
		glVertex2f((0.3 * cos((i + 1) * 2 * 3.14159265 / 60)), (0.2 * sin((i + 1) * 2 * 3.14159265 / 60)));

	}
	glEnd();
}
void petals1() {
	glTranslatef(0.35, 0.35, 0);

	float sine, cosine;
	glColor3f(1, 1, 1);
	for (int i = 0; i < 60; i++)
	{

		cosine = (0.25 * cos(i * 2 * 3.14159265 / 60));
		sine = (0.15 * sin(i * 2 * 3.14159265 / 60));
		glBegin(GL_POLYGON);

		glVertex2f(cosine, sine);
		glColor3f(0.37, 0, 0.70);
		glVertex2f((0.25 * cos((i + 1) * 2 * 3.14159265 / 60)), (0.15 * sin((i + 1) * 2 * 3.14159265 / 60)));

	}
	glEnd();
}
void flowers()
{

	/// -----------1-----------
	glPushMatrix();
	glTranslatef(-0.7, -0.3, 0);
	//glRotatef(-20, 0, 0, 1);
	petals();
	glPopMatrix();
	//---------2-----------
	glPushMatrix();
	glTranslatef(-0.65, -0.6, 0);
	glRotatef(10, 0, 0, 1);
	petals();
	glPopMatrix();

	//--------3----------------
	glPushMatrix();
	glTranslatef(-0.32, -0.85, 0);
	glRotatef(40, 0, 0, 1);
	petals();
	glPopMatrix();
	//----------4-------------

	glPushMatrix();
	glTranslatef(0.1, -0.9, 0);
	glRotatef(70, 0, 0, 1);
	petals();
	glPopMatrix();
	//-----------5-------------

	glPushMatrix();
	glTranslatef(0.45, -0.80, 0);
	glRotatef(95, 0, 0, 1);
	petals();
	glPopMatrix();
	//-----------6------------

	glPushMatrix();
	glTranslatef(0.8, -0.45, 0);
	glRotatef(125, 0, 0, 1);
	petals();
	glPopMatrix();
	//----------7--------------

	glPushMatrix();
	glTranslatef(0.95, -0.15, 0);
	glRotatef(140, 0, 0, 1);
	petals();
	glPopMatrix();
	//----------8--------------

	glPushMatrix();
	glTranslatef(0.90, 0.25, 0);
	glRotatef(170, 0, 0, 1);
	petals();
	glPopMatrix();
	//----------9--------------

	glPushMatrix();
	glTranslatef(0.65, 0.60, 0);
	glRotatef(200, 0, 0, 1);
	petals();
	glPopMatrix();
	//----------10--------------
	glPushMatrix();
	glTranslatef(0.30, 0.85, 0);
	glRotatef(220, 0, 0, 1);
	petals();
	glPopMatrix();
	//----------11--------------
	glPushMatrix();
	glTranslatef(-0.20, 0.85, 0);
	glRotatef(260, 0, 0, 1);
	petals();
	glPopMatrix();
	//----------12--------------
	glPushMatrix();
	glTranslatef(-0.75, 0.35, 0);
	glRotatef(310, 0, 0, 1);
	petals();
	glPopMatrix();
	//----------1--------------
	glPushMatrix();
	glTranslatef(-0.65, -0.3, 0);
	//glRotatef(330, 0, 0, 1);
	petals1();
	glPopMatrix();
	//----------2--------------
	glPushMatrix();
	glTranslatef(-0.48, -0.65, 0);
	glRotatef(25, 0, 0, 1);
	petals1();
	glPopMatrix();
	//----------3--------------
	glPushMatrix();
	glTranslatef(0.07, -0.84, 0);
	glRotatef(65, 0, 0, 1);
	petals1();
	glPopMatrix();
	//----------4--------------
	glPushMatrix();
	glTranslatef(0.6, -0.65, 0);
	glRotatef(100, 0, 0, 1);
	petals1();
	glPopMatrix();
	//----------5--------------
	glPushMatrix();
	glTranslatef(0.85, -0.15, 0);
	glRotatef(135, 0, 0, 1);
	petals1();
	glPopMatrix();
	//----------5--------------
	glPushMatrix();
	glTranslatef(0.85, 0.25, 0);
	glRotatef(165, 0, 0, 1);
	petals1();
	glPopMatrix();
	//----------5--------------
	glPushMatrix();
	glTranslatef(0.60, 0.65, 0);
	glRotatef(200, 0, 0, 1);
	petals1();
	glPopMatrix();
	//----------5--------------

	glPushMatrix();
	glTranslatef(0.10, 0.85, 0);
	glRotatef(235, 0, 0, 1);
	petals1();
	glPopMatrix();
	//----------5--------------
	glPushMatrix();
	glTranslatef(-0.50, 0.63, 0);
	glRotatef(285, 0, 0, 1);
	petals1();
	glPopMatrix();
	//----------5--------------
	glPushMatrix();
	glTranslatef(-0.70, 0.15, 0);
	glRotatef(320, 0, 0, 1);
	petals1();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.11, -0.15, 0);
	flower();
	glPopMatrix();

	glFlush();


}

void cloud(float x, float y, float z, float sx = 1, float sy = 1, float sz = 1) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(sx, sy, sz);
	makeCircle(0.3, 0.3, -0.4151798083686, -0.0644495210819, 1, 1, 1);
	makeCircle(0.5, 0.4, -0.2, 0.2, 1, 1, 1);
	makeCircle(0.4, 0.4, 0.0927799894005, 0.2156980037483, 1, 1, 1);
	makeCircle(0.4, 0.4, 0.3606133373151, -0.0275069903351, 1, 1, 1);
	glPopMatrix();
}

void display() {

	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fofview, 1, 1, 50.0);
	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);


	glClearColor(0.57, 0.79, 0.89, 0.2);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();


	float z = Cos(th) * Cos(ph) * r; float x = Sin(th) * Cos(ph) * r; float y = Sin(ph) * r;

	gluLookAt(x, y, z, 0, 0, -0.2, 0, 1, 0);


	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	light();
	glPopMatrix();



	GLfloat Ma[] = { 0, 0, 0, 1.0 };
	GLfloat Md[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat Ms[] = { 1.0, 1.0, 1.0, 1.0 };


	glMaterialfv(GL_FRONT, GL_AMBIENT, Ma);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Md);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Ms);
	glMaterialf(GL_FRONT, GL_SHININESS, 120);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);


	goTo();


	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glScalef(1.3, 1.3, 1.3);
	glTranslatef(0, 0, -0.1);
	glBindTexture(GL_TEXTURE_2D, backC);
	back();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0.65, 0.42f, -0.14);
	glScalef(0.1, 0.1, 0.1);

	flowers();
	glPopMatrix();


	glScalef(0.75, 0.75, 0.75);
	glTranslatef(0, 0, 0);

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, -0.03, -0.18);
	Maze();
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, selected);

	glPushMatrix();
	glTranslatef(2.5, 1.5, 2);
	glScalef(-1, 1, 1);
	qustionWindwo();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);



	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, -0.19);
	butterfly(curretposX, curretposY, butterfly_pos[stage][0][2]);

	glPopMatrix();

	sun(0.2, 0.2, -2, 2, 1, 1, 1, 0);
	cloud(-3, 1, 1);
	cloud(2, 2, 2);

	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	glutSwapBuffers();


}

void reshapeFunc(GLint new_width, GLint new_hight)
{
	glViewport(0, 0, 500, 500);
	win_width = new_width;
	win_hight = new_hight;
	glMatrixMode(GL_PROJECTION);
	gluPerspective(30, new_width / new_hight, 0.1, 50.0);

}

void init() {
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1, 3, -1, 1, -6, 6);
	glMatrixMode(GL_MODELVIEW);
	myTexture1 = load(image1Path);
	myTexture2 = load(image2Path);
	myTexture3 = load(image3Path);
	myTexture4 = load(image4Path);
	myTexture5 = load(image5Path);
	myTexture6 = load(image6Path);
	myTexture7 = load(image7Path);
	myTexture8 = load(image8Path);

	TT[0] = myTexture1;
	TT[1] = myTexture2;
	TT[2] = myTexture3;
	TT[3] = myTexture4;
	TT[4] = myTexture5;
	TT[5] = myTexture6;
	TT[6] = myTexture7;
	TT[7] = myTexture8;

	selected = TT[0];
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(1200, 600);

	glutInitWindowPosition(10, 10);

	glutCreateWindow("Maze Project");
	init();

	glutDisplayFunc(display);
	glutTimerFunc(0, redisplay, 0);
	
	backC = load("background.png");
	cubeTex = load("cube.jpg");
	glutMainLoop();

}