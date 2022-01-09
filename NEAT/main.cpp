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
#include "genome.h"

#define COLUMNS 60
#define ROWS 60

#define FPS 180

Bird *newbird = nullptr;

vector<int> posY(NEAT::pop_size);

void timer_callback(int);
void display_callback();
void reshape_callback(int, int);
void keyboard_callback(int, int, int);
void display_phenotype();

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

	/*while (true)
	{
		drawBars();

		newbird->drawBall();

		Sleep(1);
	}*/
	
	//p = flappy_bird(10, birds);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(350, 40);

	glutCreateWindow("FLIP FLOP");
	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);
	glutTimerFunc(0, timer_callback, 0);
	
	glutCreateWindow("Best Phenotype");
	glutDisplayFunc(display_phenotype);
	glutReshapeFunc(reshape_callback);
	glutTimerFunc(0, timer_callback, 0);
	//glutReshapeFunc(reshape_callback);
	//glutTimerFunc(0, timer_callback, 0);

	//glutSpecialFunc(keyboard_callback);

	init();
	glutMainLoop();

	cout << "its finish!" << endl;
	if (p)
		delete p;
	
	return 0;
}

struct NODE_VEC
{
public:
	float posX;
	float posY;
	int id = -1;
};

int max_fitness = 0;

Organism *org = nullptr;
bool isNew = false;

void display_phenotype()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Organism *newOrg;
	GetMaxOrg(newOrg);
	
#define MAX_WIDTH 60
#define MAX_HEIGHT 60

	if (newOrg && org == nullptr || org != newOrg)
	{
		org = newOrg;
	}

	if (org != nullptr)
	{
		mutex &m = org->GetMutex();
		m.lock();
		vector<NNode*> nodes = org->gnome->nodes;
		vector<Gene*> genes = org->gnome->genes;
		vector<NNode*> inputs = org->net->inputs;
		vector<NNode*> outputs = org->net->outputs;
		vector<NODE_VEC> phenoNode;

		glColor3f(1, 1, 0);
		for (int k = 1; k < inputs.size(); k++) //input
		{
			glBegin(GL_POLYGON);
			double radius = 1;
			double ori_x = MAX_WIDTH / (inputs.size()) * k;
			double ori_y = 10;
			for (int i = 0; i <= 300; i++) {
				double angle = 2 * 3.14159 * i / 300;
				double x = cos(angle) * radius;
				double y = sin(angle) * radius;
				glVertex2d(ori_x + x, ori_y + y);
			}
			glEnd();
			NODE_VEC node;
			node.id = inputs[k]->node_id;
			node.posX = ori_x;
			node.posY = ori_y;
			phenoNode.push_back(node);
		}

		for (int k = 0; k < outputs.size(); k++) //output
		{
			glBegin(GL_POLYGON);
			double radius = 1;
			double ori_x = MAX_WIDTH / (outputs.size() + 1) * (k + 1);
			double ori_y = 50;
			for (int i = 0; i <= 300; i++) {
				double angle = 2 * 3.14159 * i / 300;
				double x = cos(angle) * radius;
				double y = sin(angle) * radius;
				glVertex2d(ori_x + x, ori_y + y);
			}
			glEnd();
			NODE_VEC node;
			node.id = outputs[k]->node_id;
			node.posX = ori_x;
			node.posY = ori_y;
			phenoNode.push_back(node);
		}

		vector<NNode*> hiddenOrBias;
		for (auto i : nodes)
		{
			if (i->gen_node_label == nodeplace::BIAS || i->gen_node_label == nodeplace::HIDDEN)
			{
				hiddenOrBias.push_back(i);
			}
		}

		for (int k = 0; k < hiddenOrBias.size(); k++)
		{
			glBegin(GL_POLYGON);
			double radius = 1;
			double ori_x = hiddenOrBias[k]->gen_node_label == nodeplace::BIAS ? 50 : MAX_WIDTH / (hiddenOrBias.size() + 1) * (k + 1);
			double ori_y = hiddenOrBias[k]->gen_node_label == nodeplace::BIAS ? 40 : 30;
			for (int i = 0; i <= 300; i++) {
				double angle = 2 * 3.14159 * i / 300;
				double x = cos(angle) * radius;
				double y = sin(angle) * radius;
				glVertex2d(ori_x + x, ori_y + y);
			}
			glEnd();
			NODE_VEC node;
			node.id = hiddenOrBias[k]->node_id;
			node.posX = ori_x;
			node.posY = ori_y;
			phenoNode.push_back(node);
		}

		for (int j = 0; j < genes.size(); j++)
		{
			//genes[j]->lnk->in_node
			NODE_VEC inNode;
			NODE_VEC outNode;
			for (int k = 0; k < phenoNode.size(); k++)
			{
				if (genes[j]->lnk->in_node->node_id == phenoNode[k].id)
				{
					inNode = phenoNode[k];
				}
				if (genes[j]->lnk->out_node->node_id == phenoNode[k].id)
				{
					outNode = phenoNode[k];
				}
			}

			if (inNode.id != -1 && outNode.id != -1)
			{
				glBegin(GL_LINES);
				glVertex2f(inNode.posX, inNode.posY);
				glVertex2f(outNode.posX, outNode.posY);
				glEnd();
			}
		}
		m.unlock();
		//glBegin(GL_POLYGON);                        // Middle circle
		//double radius = 1;
		//double ori_x = 20;                         // the origin or center of circle
		//double ori_y = 20;
		//for (int i = 0; i <= 300; i++) {
		//	double angle = 2 * 3.14159 * i / 300;
		//	double x = cos(angle) * radius;
		//	double y = sin(angle) * radius;
		//	glVertex2d(ori_x + x, ori_y + y);
		//}
		//glEnd();
	}

	/* Lines*/
	/*
	glBegin(GL_LINES);
	glVertex2f(10, 10);
	glVertex2f(20, 20);
	glEnd();
	*/

	glutPostRedisplay();
	glutSwapBuffers();
}

void display_callback()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//drawGrid();
	newbird->drawBars();

	//for(auto &i : birds)
	//	i->drawBall();
	newbird->drawBall();
	glutPostRedisplay();
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
