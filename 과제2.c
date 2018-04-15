// ConsoleApplication2.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"
#include<glut.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

static GLfloat PI = 3.141592,angle=0,angley=0;
static int tempx, tempy;
GLfloat eye[3] = { 0,0,0 }, at[3] = { 0,0,11 }, up[3] = { 0,1,0 };


void init() { // openGL 초기화
			  //srand(time(0));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // 정방행렬 = 단위행렬
	glClearColor(1.0, 1.0, 1.0, 1.0); // (r g b a(투명도)(1이 불투명)순서)(1이 최대값 가장 강한 색)
	glColor3f(0.0, 0.0, 1.0); // 붓의 색 선택(1 0 0 --> 빨간색)
	glFrustum(-1,1,-1,1,1,20); // 좌표계 크기 설정 (left,right,bottom,top)
											  //화면에보이는 좌표 범위 
											  //glFrustum(-150, 150, -150, 150, -500, 500);
											  //gluPerspective(85.0, 1.0, 2.0, 20.0);
	glEnable(GL_DEPTH_TEST);   // 테스트한 결과는 DEPTH_BUFFER에 저장된다.
							   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glMatrixMode(GL_MODELVIEW);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();		// 이게 없으면 2도 회전된 행렬에 또 4도 회전 6도 회전..
							//gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);
							//glRotatef(theta[0], 1.0, 0, 0); //theta만큼 y축 기준으로 회전
							//glRotatef(theta[1], 0, 1.0, 0);
							//glRotatef(theta[2], 0, 0, 1);
	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);
	
	glPushMatrix();
	glTranslatef(0, 0, 5);            // 정면
	glColor3f(1.0, 0.0, 0.0);
	glutWireTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -5);          // 뒤쪽
	glColor3f(1.0, 0.0, 1.0);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 0, 0);           // 왼쪽
	glColor3f(0.0, 1.0, 0.0);
	glutWireCone(1, 1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, 0, 0);
	glColor3f(0.0, 0.0, 1.0);         // 오른쪽
	glutWireTorus(0.5,0.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 5, 0);          // 위쪽
	glColor3f(0.0, 1.0, 1.0);
	glutWireSphere(1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -5, 0);         // 아래쪽
	glColor3f(1.0, 0.5, 0.5);
	glutWireIcosahedron();
	glPopMatrix();

	glFlush();

}


void keyboard_handler(unsigned char key, int x, int y) {
	if (key == 'w') {        // 앞으로->물체크게           // 고개를 튼 방향에서도 앞으로 이동 -> 원 그려서 생각
		eye[2] += 0.1*cos(angle*PI / 180.0);			   // z축으로 이동한 각의 cos만큼 이동
		eye[0] += 0.1*sin(angle*PI / 180.0);			   // x축으로 이동한 각의 sin만큼 이동
		at[2] += 0.1*cos(angle*PI / 180.0);				   // 보는 점도 위와 마찬가지
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
		eye[0] += 0.1*sin((90-angle)*PI / 180.0);            
		at[2] += 0.1*-cos((90 - angle)*PI / 180.0);
		at[0] += 0.1*sin((90 - angle)*PI / 180.0);
	}
	if (key == 'd') {      // 오른쪽이동-> 물체 왼쪽
		eye[2] += 0.1*cos((90 - angle)*PI / 180.0);
		eye[0] += 0.1*-sin((90 - angle)*PI / 180.0);
		at[2] += 0.1*cos((90 - angle)*PI / 180.0);
		at[0] += 0.1*-sin((90 - angle)*PI / 180.0);
	}
	
	printf("%lf %lf %lf %lf\n", eye[0], eye[2], at[0], at[2]);
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
		if(angle<-360)
			angle+=360;
		if (!(angley + (float)(y - tempy) / 25.0 > 90 || angley + (float)(y - tempy) / 25.0 < -90)) {     // y축으로 90, -90도 이상 넘어가면 안됨 목의 한계
			angley += (float)(y - tempy) / 25.0;
			at[1] = eye[1]+ (angley*PI / 180);
		}
			
		at[0] = eye[0] + sin(angle*PI / 180);         // 고개 틀면 눈은 그대로 시야 at만 바뀜
		at[2] = eye[2] + cos(angle*PI / 180);
		
		printf("%lf %lf\n", angle, angley);
		printf("at0 : %lf at1 %lf at2 %lf\n", at[0], at[1], at[2]);
	}
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, (char**)argv); // 초기화
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500); // 윈도우크기초기화(도화지크기)(실행하면 나오는 화면)
	glutCreateWindow("FPS"); // 윈도우생성 
												 //glutIdleFunc(spin_cube);
	glutKeyboardFunc(keyboard_handler);
	glutMouseFunc(mouse_handler);
	glutDisplayFunc(display);  // os가 언제 화면을 보여줄지(parameter를 그릴것)
							   //glutTimerFunc(delay, timer, 0);

	init(); // openGL 초기화
	glutMainLoop(); // event queue에 어떤 event가 들어있는지 검사하는 loop시작

	return 0;
}