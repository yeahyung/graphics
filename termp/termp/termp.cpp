#include "stdafx.h"
#include<conio.h>
#include <stdlib.h>
#include <glut.h>
#include <math.h>
#include <string>
#include<Windows.h>
#include <time.h>
#include<mmsystem.h>
#include <fstream>

#pragma warning(disable:4996)

#pragma comment(lib,"winmm.lib") 
#define SIZE 7
#define TURN 1.0
#define SOUND "../scream.wav"
#define SOUND1 "../terrible.wav"
GLuint loadTex(const char*);
GLuint tex[3], picture[3];
static GLfloat PI = 3.141592, angle = 0, angley = 0;
static int tempx, tempy, miromap[20][20];
GLfloat eye[3] = { 1,0,2 }, at[3] = { 1,0,3 }, up[3] = { 0,1,0 };
float a = 0.0, b = 1.0;
static int Mx = 0, My = 0;
GLfloat angX = 120.0, angY = 0.0, tmp_ang = 0.0;
GLdouble X = 3.0, Y = 0.0, Z = 0.8;
int cutoff = 10, w, h;
int warning_sign = 0;
float ambientL = 0.2;
float diffuseL = 0.7;
float specularL = 0.9;
struct point
{
	float min_x = 0;
	float min_z = 0;
	float max_x = 0;
	float max_z = 0;
	bool warning = false;
};
point static_block[SIZE][SIZE];

/*AUX_RGBImageRec *LoadBMPFile(const char *filename) {
FILE *hfile = NULL;
if (!filename)
return NULL;
fopen_s(&hfile,filename, "r");
if (hfile) {
fclose(hfile);
return auxDIBImageLoad((LPCWSTR)(const char*)"input.bmp");
}
return NULL;
}*/

void Lantern()
{
	GLfloat Direction[] = { 0, 0, 1 }; // 주변 얼마나 비출지 범위
	GLfloat lightPos[] = { 0, 0, 0, 1 };
	GLfloat specref[] = { 1,1,1,1 };

	GLfloat AmbientLight[] = { ambientL, ambientL, ambientL, ambientL };
	GLfloat DiffuseLight[] = { diffuseL, diffuseL, diffuseL, diffuseL };
	GLfloat SpecularLight[] = { specularL, specularL, specularL, specularL };
	lightPos[0] = eye[0] - 4 * (at[0] - eye[0]); lightPos[2] = eye[2] - 4 * (at[2] - eye[2]);
	Direction[0] = (at[0] - eye[0]); Direction[2] = (at[2] - eye[2]);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Direction);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff - warning_sign);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.9);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1);

	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularLight);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glEnable(GL_LIGHT1);
}
void draw_block() {
	glPushMatrix();
	{
		glNormal3f(0, 1, 0);

		glColor3f(0.5, 0.35, 0.25);
		glTranslatef(0, 0, 0);
		glutSolidCube(1);

		glColor3f(0.7, 0.4, 0.3);
		glTranslatef(0, 1, 0);
		glutSolidCube(1);

		glColor3f(0.6, 0.35, 0.25);
		glTranslatef(0, 1, 0);
		glutSolidCube(1);
	}
	glPopMatrix();
}
void Line_block()
{
	glColor3f(0.7, 0.7, 0.7);

	glPushMatrix();
	{
		glNormal3f(0, 1, 0);
		glTranslatef(0, 1, 0);
		glutSolidCube(0.1);

		glTranslatef(0, 1, 0);
		glutSolidCube(1);

		glTranslatef(0, 1, 0);
		glutSolidCube(1);
	}
	glPopMatrix();
}
void draw_ghost() {
	glEnable(GL_BLEND);
	glPushMatrix();
	{
		glBlendFunc(GL_ONE, GL_ONE);
		glRotatef(90, 0, 1, 0);
		glColor4f(0.5f, 1.0f, 1.0f, 1.0f);

		glTranslatef(0.0f, 0.75f, 0.0f);
		glutSolidSphere(0.40f, 20, 20);

		glTranslatef(0.0f, 0.55f, 0.0f);
		glutSolidSphere(0.20f, 20, 20);

		glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(0.05f, 0.10f, 0.18f);
		glutSolidSphere(0.05f, 10, 10);
		glTranslatef(-0.1f, 0.0f, 0.0f);
		glutSolidSphere(0.05f, 10, 10);
		glPopMatrix();

		glColor3f(1.0f, 0.5f, 0.5f);
		glutSolidCone(0.08f, 0.45f, 10, 2);
	}
	glPopMatrix();
	glDisable(GL_BLEND);

	//glEnd();
	glLineWidth(1); // 블럭 위치

}
void draw_ghostt(float x1, float x2, float y1, float y2, float z1, float z2) {
	//AUX_RGBImageRec *imgRec = auxDIBImageLoad(LPCWSTR("input.bmp"));
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, picture[0]); //<1>
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glNormal3f(1.0f, 1.0f, 1.0f);
	glColor4f(0.5, 0.5, 0.5, 0.3);
	glTexCoord2f(a, a);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(b, a);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(b, b);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(a, b);
	glVertex3f(x1, y2, z1);// 오른쪽

	glColor4f(0.5, 0.5, 0.5, 0.3);
	glTexCoord2f(a, a);
	glVertex3f(x1, y1, z2);
	glTexCoord2f(b, a);
	glVertex3f(x2, y1, z2);
	glTexCoord2f(b, b);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(a, b);
	glVertex3f(x1, y2, z2);//왼쪽 면

	glColor4f(0.5, 0.5, 0.5, 0.3);
	glTexCoord2f(a, a);
	glVertex3f(x1, y1, z2);
	glTexCoord2f(b, a);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(b, b);
	glVertex3f(x1, y2, z1);
	glTexCoord2f(a, b);
	glVertex3f(x1, y2, z2);//정면

	glColor4f(0.5, 0.5, 0.5, 0.3);
	glTexCoord2f(a, a);
	glVertex3f(x2, y1, z2);
	glTexCoord2f(b, a);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(b, b);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(a, b);
	glVertex3f(x2, y2, z2);
	//뒷면

	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
void draw_endline(float x1, float x2, float y1, float y2, float z1, float z2) {
	//AUX_RGBImageRec *imgRec = auxDIBImageLoad(LPCWSTR("input.bmp"));
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, picture[0]); //<1>
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBlendFunc(GL_SRC_ALPHA, 1);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 1.0f, 1.0f);

	glColor4f(0.5, 0.5, 0.5, 0.3);
	glTexCoord2f(a, a);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(b, a);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(b, b);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(a, b);
	glVertex3f(x1, y2, z1);// 오른쪽

	glColor4f(0.5, 0.5, 0.5, 0.3);
	glTexCoord2f(a, a);
	glVertex3f(x1, y1, z2);
	glTexCoord2f(b, a);
	glVertex3f(x2, y1, z2);
	glTexCoord2f(b, b);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(a, b);
	glVertex3f(x1, y2, z2);//왼쪽 면

	glColor4f(0.5, 0.5, 0.5, 0.3);
	glTexCoord2f(a, a);
	glVertex3f(x1, y1, z2);
	glTexCoord2f(b, a);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(b, b);
	glVertex3f(x1, y2, z1);
	glTexCoord2f(a, b);
	glVertex3f(x1, y2, z2);//정면

	glColor4f(0.5, 0.5, 0.5, 0.3);
	glTexCoord2f(a, a);
	glVertex3f(x2, y1, z2);
	glTexCoord2f(b, a);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(b, b);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(a, b);
	glVertex3f(x2, y2, z2);
	//뒷면

	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
void draw_wall(float x1, float x2, float y1, float y2, float z1, float z2) {
	//AUX_RGBImageRec *imgRec = auxDIBImageLoad(LPCWSTR("input.bmp"));
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, picture[1]); //<1>
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glColor3f(1, 1, 1);
	glTexCoord2f(a, a);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(b, a);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(b, b);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(a, b);
	glVertex3f(x1, y2, z1);// 오른쪽

	glColor3f(1, 1, 1);
	glTexCoord2f(a, a);
	glVertex3f(x1, y1, z2);
	glTexCoord2f(b, a);
	glVertex3f(x2, y1, z2);
	glTexCoord2f(b, b);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(a, b);
	glVertex3f(x1, y2, z2);//왼쪽 면

	glColor3f(1, 1, 1);
	glTexCoord2f(a, a);
	glVertex3f(x1, y1, z2);
	glTexCoord2f(b, a);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(b, b);
	glVertex3f(x1, y2, z1);
	glTexCoord2f(a, b);
	glVertex3f(x1, y2, z2);//정면

	glColor3f(1, 1, 1);
	glTexCoord2f(a, a);
	glVertex3f(x2, y1, z2);
	glTexCoord2f(b, a);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(b, b);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(a, b);
	glVertex3f(x2, y2, z2);
	//뒷면	
	glDisable(GL_TEXTURE_2D);
	glEnd();
	glPushMatrix();
	{	

		glColor3f(0.5, 0.35, 0.25);
		glTranslatef(x1+0.5, 1, z1+0.5);
		glColor3f(0.7, 0.4, 0.3);
		glutSolidCube(1);
		glTranslatef(0, 1, 0);	
		glColor3f(0.6, 0.35, 0.25);
		glutSolidCube(1);
	}
	glPopMatrix();
	

}
void makemap() {       // 0 빈 것 1 벽돌 2 벽지 3 출발지 4 도착지 5 귀신
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			glPushMatrix(); {                   // 바닥
				glTranslatef(i * 1, 0, j * 1);
				glTranslatef(0, -1, 0);
				glColor3f(0.3, 0.5, 0.4);
				glutSolidCube(1);
			}
			glPopMatrix();

			if (miromap[i][j] == 1) { // 벽
				glPushMatrix();
				{
					glTranslatef(i * 1, 0, j * 1);
					draw_block();
				}
				glPopMatrix();

				glColor3f(1, 0, 0);
				glLineWidth(1);
			}
			if (miromap[i][j] == 2) { // 벽지
				glPushMatrix();
				{
					//glTranslatef(i * 1, 1, j * 1);
					draw_wall(i - 0.5, i + 0.5, -0.5, 0.5, j - 0.5, j + 0.5);
				}
				glPopMatrix();

				glColor3f(1, 0, 0);
				glLineWidth(1);
			}
			if (miromap[i][j] == 4) {    //도착지점
				draw_endline(i - 0.5, i + 0.5, -0.5, 0.5, j - 0.5, j + 0.5);
			}
			if (miromap[i][j] == 5) {     // 귀신
				glPushMatrix(); {
					//glTranslatef(i*1, 0, j*1);
					draw_ghostt(i - 0.5, i + 0.5, -0.5, 0.5, j - 0.5, j + 0.5);
				}
				glPopMatrix();
				//glLineWidth(1);
			}
		}

	}
	glFlush();
}
void open_file() {
	FILE *fp;
	fopen_s(&fp, "miro.txt", "r");
	if (fp == NULL) {
		printf("파일 열기 실패\n");
		return;
	}
	printf("완료\n");
	int cha;
	while (feof(fp) == 0) {
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				fscanf_s(fp, "%d ", &cha);
				miromap[i][j] = cha;
				if (cha == 3) {
					eye[0] = i; eye[2] = j;
					at[0] = i; at[2] = j + 1;
					printf("%d %d\n", i, j);
				}
			}
		}
	}
	fclose(fp);
}

bool collision()
{
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if ((static_block[i][j].max_x > eye[0] && static_block[i][j].min_x < eye[0]) && (static_block[i][j].max_z<eye[2] - 5 && static_block[i][j].min_z >eye[2] - 5))
			{
				printf("%lf %lf %lf %lf", static_block[i][j].max_x, static_block[i][j].min_x, static_block[i][j].max_z, static_block[i][j].min_z);
				return false;
			}
		}
	}
	return true;
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	/*glRotatef(angX, 0.0, 1.0, 0.0);
	glRotatef(angY, 1.0, 0.0, 0.0);
	glTranslatef(X, Y, Z);*/
	//effectonoff();
	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);
	makemap();
	//MainLight();
	Lantern();
	//Plane();
}

void keyboard_handler(unsigned char key, int x, int y) {
	/*

	if (key == 'w') {        // 앞으로->물체크게           // 고개를 튼 방향에서도 앞으로 이동 -> 원 그려서 생각
	eye[2] += 0.1*cos(angle*PI / 180.0);            // z축으로 이동한 각의 cos만큼 이동
	eye[0] += 0.1*sin(angle*PI / 180.0);            // x축으로 이동한 각의 sin만큼 이동
	at[2] += 0.1*cos(angle*PI / 180.0);               // 보는 점도 위와 마찬가지
	at[0] += 0.1*sin(angle*PI / 180.0);
	}
	if (key == 's') {       //뒤로감 ->물체 작게보임
	eye[2] += 0.1*-cos(angle*PI / 180.0);
	eye[0] += 0.1*-sin(angle*PI / 180.0);
	at[2] += 0.1*-cos(angle*PI / 180.0);
	at[0] += 0.1*-sin(angle*PI / 180.0);
	}
	if (key == 'a') {       //왼쪽이동 -> 물체 오른쪽        // 고개를 a만큼 튼 상태에서 좌우 이동 z축과의 각도 = 90-a
	eye[2] += 0.1*-cos((90 - angle)*PI / 180.0);
	eye[0] += 0.1*sin((90 - angle)*PI / 180.0);
	at[2] += 0.1*-cos((90 - angle)*PI / 180.0);
	at[0] += 0.1*sin((90 - angle)*PI / 180.0);
	}
	if (key == 'd') {      // 오른쪽이동-> 물체 왼쪽
	eye[2] += 0.1*cos((90 - angle)*PI / 180.0);
	eye[0] += 0.1*-sin((90 - angle)*PI / 180.0);
	at[2] += 0.1*cos((90 - angle)*PI / 180.0);
	at[0] += 0.1*-sin((90 - angle)*PI / 180.0);
	}*/


	if (angle == 0) {
		if (key == 'w') {
			if (miromap[(int)eye[0]][(int)eye[2] + 1] == 1|| miromap[(int)eye[0]][(int)eye[2] + 1] == 2)
				printf("충돌\n");
			else if (miromap[(int)eye[0]][(int)eye[2] + 1] == 5) {
				PlaySound(TEXT("scream.wav"), NULL, SND_SYNC | SND_ALIAS);
				printf("충돌\n"); PlaySound(TEXT("terrible.wav"), NULL, SND_ASYNC | SND_LOOP);
			}
			else {
				at[2]++; eye[2]++;
			}
		}
		if (key == 's') {
			if (miromap[(int)eye[0]][(int)eye[2] - 1] == 1|| miromap[(int)eye[0]][(int)eye[2] - 1] == 2)
				printf("충돌\n");
			else if (miromap[(int)eye[0]][(int)eye[2] - 1] == 5) {
				PlaySound(TEXT("scream.wav"), NULL, SND_SYNC | SND_ALIAS);
				printf("충돌\n"); PlaySound(TEXT("terrible.wav"), NULL, SND_ASYNC | SND_LOOP);
			}
			else {
				at[2]--; eye[2]--;
			}
		}
		if (key == 'd') {
			at[0]--; at[2]--; angle = 270;
		}
		if (key == 'a') {

			at[0]++; at[2]--; angle = 90;
		}
		printf("%lf %lf %lf %lf\n", eye[0], eye[2], at[0], at[2]);
		return;
	}
	if (angle == 90) {
		if (key == 'w') {
			if (miromap[(int)eye[0] + 1][(int)eye[2]] == 1|| miromap[(int)eye[0] + 1][(int)eye[2]] == 2)
				printf("충돌\n");
			else if (miromap[(int)eye[0] + 1][(int)eye[2]] ==5) {
				PlaySound(TEXT("scream.wav"), NULL, SND_SYNC | SND_ALIAS);
				printf("충돌\n"); PlaySound(TEXT("terrible.wav"), NULL, SND_ASYNC | SND_LOOP);
			}
			else {
				at[0]++; eye[0]++;
			}
		}
		if (key == 's') {
			if (miromap[(int)eye[0] - 1][(int)eye[2]] == 1|| miromap[(int)eye[0] - 1][(int)eye[2]] == 2)
				printf("충돌\n");
			else if(miromap[(int)eye[0] - 1][(int)eye[2]] == 5) {
				PlaySound(TEXT("scream.wav"), NULL, SND_SYNC | SND_ALIAS);
				printf("충돌\n"); PlaySound(TEXT("terrible.wav"), NULL, SND_ASYNC | SND_LOOP);
			}
			else {
				at[0]--; eye[0]--;
			}
		}
		if (key == 'd') {
			at[0]--; at[2]++; angle = 0;
		}
		if (key == 'a') {
			at[0]--; at[2]--; angle = 180;
		}
		printf("%lf %lf %lf %lf\n", eye[0], eye[2], at[0], at[2]);
		return;
	}
	if (angle == 180) {
		if (key == 'w') {
			if (miromap[(int)eye[0]][(int)eye[2] - 1] == 1|| miromap[(int)eye[0]][(int)eye[2] - 1] == 2)
				printf("충돌\n");
			else if (miromap[(int)eye[0]][(int)eye[2] - 1] == 5) {
				PlaySound(TEXT("scream.wav"), NULL, SND_SYNC|SND_ALIAS);
				printf("충돌\n"); PlaySound(TEXT("terrible.wav"), NULL, SND_ASYNC | SND_LOOP);
			}
			else {
				at[2]--; eye[2]--;
			}
		}
		if (key == 's') {
			if (miromap[(int)eye[0]][(int)eye[2] + 1] == 1|| miromap[(int)eye[0]][(int)eye[2] + 1] == 2)
				printf("충돌\n");
			else {
				at[2]++; eye[2]++;
			}
		}
		if (key == 'd') {
			at[0]++; at[2]++; angle = 90;
		}
		if (key == 'a') {
			at[0]--; at[2]++; angle = 270;
		}
		printf("%lf %lf %lf %lf\n", eye[0], eye[2], at[0], at[2]);
		return;
	}
	if (angle == 270) {
		if (key == 'w') {
			if (miromap[(int)eye[0] - 1][(int)eye[2]] == 1 || miromap[(int)eye[0] - 1][(int)eye[2]] == 2)
				printf("충돌\n");
			else if (miromap[(int)eye[0] - 1][(int)eye[2]] == 5) {
				PlaySound(TEXT("scream.wav"), NULL, SND_SYNC | SND_ALIAS);
				printf("충돌\n"); PlaySound(TEXT("terrible.wav"), NULL, SND_ASYNC | SND_LOOP);
			}
			else {
				at[0]--; eye[0]--;
			}
		}
		if (key == 's') {
			if (miromap[(int)eye[0] + 1][(int)eye[2]] == 1)
				printf("충돌\n");
			else {
				at[0]++; eye[0]++;
			}
		}
		if (key == 'd') {
			at[0]++; at[2]--; angle = 180;
		}
		if (key == 'a') {
			at[0]++; at[2]++; angle = 0;
		}
		printf("%lf %lf %lf %lf\n", eye[0], eye[2], at[0], at[2]);
		return;
	}
	//printf("%lf %lf %lf %lf %lf\n", angle,eye[0], eye[2], at[0], at[2]);
	glutPostRedisplay();
}
void mouse_handler(int btn, int state, int x, int y) { // x,y 마우스포인터 위치좌표
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		tempx = x; tempy = y;
		printf("%d %d\n", x, y);
	}
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		angle += (float)(x - tempx) / 25.0;
		if (angle > 360)
			angle -= 360;
		if (angle < -360)
			angle += 360;


		at[0] = eye[0] + sin(angle*PI / 180);         // 고개 틀면 눈은 그대로 시야 at만 바뀜
		at[2] = eye[2] + cos(angle*PI / 180);

		//printf("%lf %lf\n", angle, angley);
		//printf("at0 : %lf at1 %lf at2 %lf\n", at[0], at[1], at[2]);
	}
	glutPostRedisplay();
}

void init() { // openGL 초기화
			  //srand(time(0))
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			static_block[i][j].max_x += (i + 1) * 1;
			static_block[i][j].max_z += (j + 1) * 1;
			static_block[i][j].min_x += i * 1;
			static_block[i][j].min_z += j * 1;
			printf("%lf %lf %lf %lf\n", static_block[i][j].max_x, static_block[i][j].min_x, static_block[i][j].max_z, static_block[i][j].min_z);
		}
	} // 블럭 초기화
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // 정방행렬 = 단위행렬
	glutKeyboardFunc(keyboard_handler);
	//	glutMouseFunc(mouse_handler);
	glClearColor(0, 0, 0, 0); // (r g b a(투명도)(1이 불투명)순서)(1이 최대값 가장 강한 색)
	glColor3f(0.0, 0.0, 1.0); // 붓의 색 선택(1 0 0 --> 빨간색)
	gluPerspective(45, 1, 0.01, 100);
	//gluPerspective(100.0, 1.0, 0.01, 100.0);
	//glFrustum(-1, 1, -1, 1, 1, 20); // 좌표계 크기 설정 (left,right,bottom,top)
	//화면에보이는 좌표 범위 
	//glFrustum(-150, 150, -150, 150, -500, 500);
	//gluPerspective(85.0, 1.0, 2.0, 20.0);
	glEnable(GL_DEPTH_TEST);   // 테스트한 결과는 DEPTH_BUFFER에 저장된다.
							   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glMatrixMode(GL_MODELVIEW);
	//PlaySound(TEXT(SOUND), NULL, SND_ASYNC | SND_LOOP);
	/*AUX_RGBImageRec *texRec[3];
	memset(texRec, 0, sizeof(void *) * 3);
	glEnable(GL_TEXTURE_2D); //<11>
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if ((texRec[0] = LoadBMPFile("input.bmp")))
	{
	glGenTextures(1, &tex[0]);

	glBindTexture(GL_TEXTURE_2D, tex[0]); //<6>
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texRec[0]->sizeX, texRec[0]->sizeY, 0,
	GL_RGB, GL_UNSIGNED_BYTE, texRec[0]->data);

	}
	//return FALSE;

	for (int i = 0; i < 1; i++) {
	if (texRec[i]) {
	if (texRec[i]->data) free(texRec[i]->data);
	free(texRec[i]);
	}
	else
	;//return FALSE;
	}
	*/
}
char *img_read(const char * name)
{
	FILE *f;
	fopen_s(&f, name, "rb");
	if (f == NULL) exit(0);
	int of;
	fseek(f, 10, SEEK_SET);
	fread(&of, sizeof(int), 1, f);
	fseek(f, 4, SEEK_CUR);
	fread(&w, sizeof(int), 1, f);
	fread(&h, sizeof(int), 1, f);

	fseek(f, of, SEEK_SET);
	int j = 0;
	int by = ((w * 3 + 3) / 4) * 4 - (w * 3 % 4);
	char * tmp_m = (char *)malloc(sizeof(char) * by * h);
	char *m2 = (char *)malloc(sizeof(char) * w * h * 3);

	fread(tmp_m, sizeof(char) * by * h, 1, f);
	int x, y, i;
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			for (i = 0; i < 3; i++)
				m2[3 * (w*y + x) + i] = tmp_m[3 * (w*y + x) + (2 - i)];
		}
	}
	free(tmp_m);
	fclose(f);
	return m2;
}

GLuint loadTex(const char *c)
{
	char *wa = img_read(c);
	GLuint texid;

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, wa);
	free(wa);

	return texid;
}
void loadTexture() {
	picture[0] = loadTex("input.bmp");
	picture[1] = loadTex("wall.bmp");
	//picture[2] = loadTex("input.bmp");
}
int main(int argc, char *argv[])
{

	glutInit(&argc, (char**)argv); // 초기화
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600); // 윈도우크기초기화(도화지크기)(실행하면 나오는 화면)
	glutCreateWindow("Maze Runner"); // 윈도우생성 
									 //glutIdleFunc(display);
	open_file();
	loadTexture();
	glutDisplayFunc(display);  // os가 언제 화면을 보여줄지(parameter를 그릴것)
	glutIdleFunc(display);
	PlaySound(TEXT("terrible.wav"), NULL, SND_ASYNC | SND_LOOP);
	init(); // openGL 초기화
	glutMainLoop(); // event queue에 어떤 event가 들어있는지 검사하는 loop시작

	return 0;
}