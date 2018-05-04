#include "stdafx.h"
#include <glut.h>
#include<math.h>

static int mode = 0, num = 4;
typedef struct {
	GLfloat pos[4];
	GLfloat amb[4];
	GLfloat dif[4];
	GLfloat spe[4];
}Light;
typedef struct {
	GLfloat amb[4];
	GLfloat dif[4];
	GLfloat spe[4];
	GLfloat shi;
}Material;
Material mat = { { 0.2,0.0,0.0,1.0 },{ 1.0,1.0,0.0,1.0 },{ 1.0,1.0,0.0,1.0 },{ 100.0 } };

Light light = {
	{ 0.0,2.0,2.0,1.0 },{ 1.0,1.0,1.0,1.0 },{ 1.0,1.0,1.0,1.0 },{ 1.0,1.0,1.0,1.0 } };

GLfloat vertices[][3] = { { 0.58,0.58,0.58 },{ -0.58,-0.58,0.58 },{ 0.58,-0.58,-0.58 },{ -0.58,0.58,-0.58 } };
void init()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.5, 0.5, 0.5);
	glOrtho(-2.0, 2.0, -2.0, 2.0, -10.0, 10.0);
	gluLookAt(2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_POSITION, light.pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light.amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light.dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light.spe);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat.amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat.spe);
	glMaterialf(GL_FRONT, GL_SHININESS, mat.shi);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_FLAT);
}
void normal(GLfloat *n) {
	GLfloat x = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
	if (x > 0.0)
		for (int i = 0; i < 3; i++)
			n[i] /= x;
}

void triangle(GLfloat* a, GLfloat* b, GLfloat* c, int count)
{
	if (mode != 2)
		glShadeModel(GL_FLAT);
	else
		glShadeModel(GL_SMOOTH);
	if (mode == 0)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_POLYGON);

	GLfloat newver[3][3];
	if (count == num) {
		glNormal3fv(a);
		glVertex3fv(a);
		glNormal3fv(b);
		glVertex3fv(b);
		glNormal3fv(c);
		glVertex3fv(c);	
		glEnd();
	}

	if (count < num) {
		count++;
		newver[0][0] = (a[0] + b[0]);
		newver[0][1] = (a[1] + b[1]);
		newver[0][2] = (a[2] + b[2]);
		normal(newver[0]); // v1

		newver[1][0] = (a[0] + c[0]);
		newver[1][1] = (a[1] + c[1]);
		newver[1][2] = (a[2] + c[2]);
		normal(newver[1]); // v2

		newver[2][0] = (b[0] + c[0]);
		newver[2][1] = (b[1] + c[1]);
		newver[2][2] = (b[2] + c[2]);
		normal(newver[2]); // v3

		triangle(a, newver[0], newver[1], count);
		triangle(c, newver[1], newver[2], count);
		triangle(b, newver[2], newver[0], count);
		triangle(newver[0], newver[2], newver[1], count);
	}
}

void keyboard_handler(unsigned char key, int x, int y)
{
	/*xyz = light.pos+-=0.1
	ab = amb*/
	if (key == 'x')
		light.pos[0] -= 0.1;
	if (key == 'X')
		light.pos[0] += 0.1;
	if (key == 'y')
		light.pos[1] -= 0.1;
	if (key == 'Y')
		light.pos[1] += 0.1;
	if (key == 'z')
		light.pos[2] -= 0.1;
	if (key == 'Z')
		light.pos[2] += 0.1;
	if (key == 'm' || key == 'M') // mode 0->1->2->0
	{
		mode++;
		mode %= 3;
	}
	if (key == 'n') // change max recursive level by -1
		if (num > 0)
			num--;
	if (key == 'N') // change max recursive level by +1
		if (num < 7)
			num++;
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat.amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat.spe);
	glMaterialf(GL_FRONT, GL_SHININESS, mat.shi);

	triangle(vertices[0], vertices[1], vertices[2], 0);
	triangle(vertices[3], vertices[2], vertices[1], 0);
	triangle(vertices[0], vertices[3], vertices[1], 0);
	triangle(vertices[0], vertices[2], vertices[3], 0);

	glTranslatef(light.pos[0], light.pos[1], light.pos[2]);
	glutSolidSphere(0.1, 10, 10);

	glLightfv(GL_LIGHT0, GL_POSITION, light.pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light.amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light.dif);
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Sphere Shading");
	glutKeyboardFunc(keyboard_handler);
	glutDisplayFunc(display);

	init();
	glutMainLoop();
	return 0;
}