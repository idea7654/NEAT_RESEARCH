#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define NOB 10      //NOB = Number Of Bars

#include <Windows.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include "neat.h"
#include <ctime>
#include <iostream>
#include <vector>
#include <cmath>
#include <mutex>

using namespace std;

static int flag = 0;
static int prev_num = -1;
static vector<int> posBarX = { 55, 70, 85, 100, 115, 130, 145, 160, 175, 190 };
static vector<int> posBarY = { 24,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
//static int posBarX[NOB] = { 55,70,85,100,115,130,145,160,175,190 };
//static int posBarY[NOB] = { 24,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
static bool change = true;

static void Initialize_Game()
{
	flag = 0;
	prev_num = -1;
	vector<int> a = { 55, 70, 85, 100, 115, 130, 145, 160, 175, 190 };
	vector<int> b = { 24,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
	posBarX = a;
	posBarY = b;
	change = true;
}

static void random(int &num)
{
	int _max = 60 - 12;
	int _min = 1;
	num = _min + rand() % (_max - _min);
	//std::cout<<num<<std::endl;
	if (num == prev_num + 3)
	{
		if (num > 6)
		{
			num += 6;
		}
		else
		{
			num -= 5;
		}
	}
	else if (num == prev_num + 4)
	{
		if (num > 8)
		{
			num += 8;
		}
		else
		{
			num -= 7;
		}
	}
	prev_num = num;
}

static void drawBars()
{
	srand(time(NULL));
	if (flag == 0)
	{
		for (int i = 1; i < NOB; i++)
		{
			int num;
			random(num);
			posBarY[i] = num;
		}
		change = false;
		flag = 1;
	}
	//glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < NOB; i++)
	{
		for (int j = 0; j < posBarY[i]; j++)
		{
			//glRectd(posBarX[i], j, posBarX[i] + 1, j + 1);
		}
		for (int j = posBarY[i] + 12; j < 60; j++)
		{
			//glRectd(posBarX[i], j, posBarX[i] + 1, j + 1);
		}
	}
	for (int i = 0; i < NOB; i++)
	{
		posBarX[i]--;
	}
	if (posBarX[0] == 0)
	{
		for (int i = 0; i < NOB - 1; i++)
		{
			posBarX[i] = posBarX[i + 1];
			posBarY[i] = posBarY[i + 1];
		}
		posBarX[NOB - 1] = posBarX[NOB - 2] + 15;
		change = true;
	}
	if (change == true)
	{
		int num;
		random(num);
		if (num > 10)
			posBarY[NOB - 1] = num;
		else
			posBarY[NOB - 1] = num + 12;
		change = false;
	}
}

class Bird
{
private:
	int gridX;
	int gridY;

public:
	bool gameOver = false;
	int score = 0;
	int posX = 12;
	int posY = 30;
	bool isProcess = false;
	float angle_up = 0.0f;
	float angle_down = 0.0f;
	bool isStart = true;
	mutex mtx;
	//int posX = 12, posY = 30;

	Bird() { }

	void initGrid(int x, int y, int &posYRef)
	{
		gridX = x;
		gridY = y;
		posY = posYRef;
	}

	void CalculateAngle()
	{
		if (isStart)
		{
			float bottom = posBarX[0] - posX;
			float height = posBarY[0] + 12 - posY;
			float angle = atan2(height, bottom);
			angle_up = angle;

			height = posBarY[0] - posY;
			angle = atan2(height, bottom);
			angle_down = -angle;
		}
		else
		{
			float bottom = posBarX[0] - posX;
			float height = posBarY[1] + 12 - posY;
			float angle = atan2(height, bottom);
			angle_up = angle;

			height = posBarY[0] - posY;
			angle = atan2(height, bottom);
			angle_down = -angle;
		}
	}

	void drawBall()
	{
		//if (!this->gameOver)
		{
			//glColor3f(0.0, 1.0, 0.0);
			//glRectf(this->posX, this->posY, this->posX + 1, this->posY + 1);
			mtx.lock();
			CalculateAngle();
			mtx.unlock();
			if (posY < 0 || posY == gridX - 1 || posY > 80)
			{
				gameOver = true;
				Initialize_Game();
			}
			if (posX == posBarX[0] && (posY <= posBarY[0] || posY >= posBarY[0] + 12))
			{
				gameOver = true;
				Initialize_Game();
			}
			else if (posX == posBarX[0] && isStart)
			{
				score++;
				isStart = false;
			}
			else if (posX == posBarX[0])
			{
				score++;
			}
		}
		
		//else if (!isProcess && posX <= posBarX[0] - 5)
		//{
		//	isProcess = true;
		//}

		//if (isProcess)
		//{
		//	isProcess = false;
		//	//measure_fitness_flappybird();
		//	//½Å°æ¸Á ½ÇÇà!
		//}
	}
};

#endif