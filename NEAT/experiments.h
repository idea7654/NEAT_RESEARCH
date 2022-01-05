#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include "neat.h"
#include "network.h"
#include "population.h"
#include "organism.h"
#include "genome.h"
#include "species.h"

using namespace std;

using namespace NEAT;

Population *pole1_test(int gens);
bool pole1_evaluate(Organism *org);
int pole1_epoch(Population *pop, int generation, char *filename);
int go_cart(Network *net, int max_steps, int thresh); //Run input
//Move the cart and pole
void cart_pole(int action, float *x, float *x_dot, float *theta, float *theta_dot);

Population *flappy_bird(int gens, int &posY);
int measure_fitness_flappybird(Population *pop, int generation, char *filename);
bool flappybird_evaluate(Organism *org);
int try_flappybird(Network *net, int max_steps, int thresh); //Run input