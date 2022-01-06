#pragma once
#pragma warning(disable:4996)
#include <iostream>
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <algorithm>
#include <future>
#include <thread>
#include "neat.h"
#include "RandWell.h"
#include "population.h"
#include "experiments.h"

#define COLUMNS 60
#define ROWS 60

#define FPS 180

Bird *newbird = nullptr;

vector<int> posY(NEAT::pop_size);

void timer_callback(int);
void display_callback();
void reshape_callback(int, int);
void keyboard_callback(int, int, int);

void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//initGrid(COLUMNS, ROWS);
	replace(posY.begin(), posY.end(), 0, 30);
	newbird->initGrid(COLUMNS, ROWS, posY[0]);
	//for (auto &i : birds)
	//	i->initGrid(COLUMNS, ROWS, posY[0]);
}

using namespace std;

int main(int argc, char **argv)
{
	Init_WELL_RAND();
	//for (int i = 0; i < NEAT::pop_size; i++)
	//{
	//	newbird = new Bird();
	//	birds.push_back(newbird);
	//}
	newbird = new Bird();

	NEAT::Population *p = 0;

	future<void> future = async(launch::async, []() {
		flappy_bird(100, newbird);
		});

	replace(posY.begin(), posY.end(), 0, 30);
	newbird->initGrid(COLUMNS, ROWS, posY[0]);

	while (true)
	{
		drawBars();

		newbird->drawBall();

		Sleep(10);
	}
	
	
	//p = flappy_bird(10, birds);

	/*glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(350, 40);
	glutCreateWindow("FLIP FLOP");
	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);
	glutTimerFunc(0, timer_callback, 0);
	glutSpecialFunc(keyboard_callback);
	init();
	glutMainLoop();*/

	cout << "its finish!" << endl;
	if (p)
		delete p;
	
	return 0;
}

void display_callback()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//    drawGrid();
	drawBars();

	//for(auto &i : birds)
	//	i->drawBall();
	newbird->drawBall();

	glutSwapBuffers();

	/*if (birds[0]->gameOver)
	{
		char _score[10];
		itoa(birds[0]->score, _score, 10);
		char text[50] = "Your Score: ";
		strcat(text, _score);
		MessageBox(NULL, text, "Game Over", 0);
		exit(0);
	}*/

}

void reshape_callback(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void timer_callback(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer_callback, 0);
}

void keyboard_callback(int key, int, int)
{
	/*if (key == GLUT_KEY_UP)
	{
		birds[0]->posY++;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		birds[0]->posY--;
	}*/
}
