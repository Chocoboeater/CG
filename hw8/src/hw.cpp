#include "GL\glut.h"
#include <math.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
using namespace std;

class Point
{
public:
	int x, y;
	Point() {
		x = 0;
		y = 0;
	}
	Point(int _x, int _y) {
		x = _x;
		y = _y;
	}
	void setxy(int _x, int _y) {
		x = _x;
		y = _y;
	}
};

// �������
static int index = 0;

// �ߵ���ɫ
static int blue = 250;
static int red = 20;
static vector<Point> points;


void init(void)
{
	glClearColor(0.7, 0.9, 0.8, 0);
	glColor3f(0.1, 0.2, 0.3);
	glPointSize(3.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	gluOrtho2D(0.0, 600.0, 0.0, 480.0); 
}

// ���Ƶ�
void setPoint(Point p) {
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();
	glFlush();
}

// ����ֱ��
void setline(Point p1, Point p2) {
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
	glFlush();

	//cout << p1.x << "  " << p1.y << endl;
	//cout << p2.x << "  " << p2.y << endl;
}

const int M = 10007;
const int MAXN = 100;
int Combination(int n, int m)
{
	int C[MAXN + 1][MAXN + 1];
	int i, j;
	for (i = 0; i <= MAXN; ++i)
	{
		C[0][i] = 0;
		C[i][0] = 1;
	}
	for (i = 1; i <= MAXN; ++i)
	{
		for (j = 1; j <= MAXN; ++j)
			C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % M;
	}
	return C[n][m];
}

// ���Ʊ���������
Point setBezier(double t) {
	Point p;
	double x = 0, y = 0;
	int len = points.size() - 1;
	
	for (int k = 0; k <= len; k++) {
		int temp = Combination(len, k);
		x += points[k].x * pow((1 - t), len - k) * pow(t, k) * temp;
		y += points[k].y * pow((1 - t), len - k) * pow(t, k) * temp;
		// cout << points[k].x << "  " << points[k].y << endl;
	}
	//cout << x << "  " << y << endl;
	p.setxy(x, y);
	return p;
}

//display����
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

// ����¼�
void mymouseFunction(int button, int state, int x, int y) {
	glutSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT);
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) 
		{
			Point newp = Point(x, 480 - y);
			points.push_back(newp);
			index++;
			// ���õ����ɫ�����Ƶ�
			glColor3ub(2, 155, 100);
			setPoint(points[index-1]);
			// �����ߵ���ɫ��������
			glColor3ub(0.5, 0.0, 0.0);
			for (int i = 1; i < index; i++)
				setline(points[i - 1], points[i]);

			glColor3ub(red, 0, blue); // �趨���������ߵ���ɫ
			blue -= 10;
			red += 15;
			Point p_current = points[0]; //��Ϊ���
			for (double t = 0.0; t <= 1.0 && index > 1; t += 0.004)
			{
				Point P = setBezier(t);
				setline(p_current, P);
				p_current = P;
			}

		}
	}
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN && index > 1) 
		{
			points.pop_back();
			index--;
			glColor3ub(2, 155, 100);
			setPoint(points[index-1]);
			glColor3ub(0.5, 0.0, 0.0);
			for (int i = 1; i < index; i++)
				setline(points[i - 1], points[i]);

			glColor3ub(red, 0, blue);
			blue -= 10;
			red += 15;
			Point p_current = points[0];
			for (double t = 0.0; t <= 1.0 && index > 1; t += 0.004)
			{
				Point P = setBezier(t);
				setline(p_current, P);
				p_current = P;
			}

		}
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); 
	glutInitWindowSize(600, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("learnOpengl");

	init(); // ��ʼ��
	glutMouseFunc(mymouseFunction);
	glutDisplayFunc(display);
	glutMainLoop(); 
	return 0;
}