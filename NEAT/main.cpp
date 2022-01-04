#pragma once
#include <iostream>
#include "neat.h"
#include "RandWell.h"
#include "population.h"
#include "experiments.h"

using namespace std;

int main()
{
	Init_WELL_RAND();
	
	NEAT::Population *p = 0;
	p = pole1_test(1);

	cout << "its finish!" << endl;
	if (p)
		delete p;
	
	return 0;
}