#pragma once
#include <iostream>
#include <sstream>
#include "trait.h"
#include "RandWell.h"

NEAT::Trait::Trait(int id, double p1, double p2, double p3, double p4, double p5, double p6, double p7, double p8, double p9)
{
	trait_id = id;
	params[0] = p1;
	params[1] = p2;
	params[2] = p3;
	params[3] = p4;
	params[4] = p5;
	params[5] = p6;
	params[6] = p7;
	params[7] = 0;
}

NEAT::Trait::Trait(const Trait & t)
{
	for (int count = 0; count < NEAT::num_trait_params; count++)
		params[count] = 0;
	trait_id = 0;
}

NEAT::Trait::Trait(Trait * t)
{
	for (int count = 0; count < NEAT::num_trait_params; count++)
		params[count] = (t->params)[count];

	trait_id = t->trait_id;
}

NEAT::Trait::Trait(const char * argline)
{
	std::stringstream ss(argline);
	ss >> trait_id;

	for (int count = 0; count < NEAT::num_trait_params; count++) {
		ss >> params[count];
	}
}

NEAT::Trait::Trait(Trait * t1, Trait * t2)
{
	for (int count = 0; count < NEAT::num_trait_params; count++)
		params[count] = (((t1->params)[count]) + ((t2->params)[count])) / 2.0;
	trait_id = t1->trait_id;
}

void NEAT::Trait::mutate()
{
	for (int count = 0; count < NEAT::num_trait_params; count++) {
		if (randbtn(0.0, 1.0) > NEAT::trait_param_mut_prob) { //trait_param_mut_prob = 0.5
			params[count] += (isEven()*randbtn(0.0, 1.0))*NEAT::trait_mutation_power; //trait_mutation_power = 1.0
			if (params[count] < 0) params[count] = 0;
			if (params[count] > 1.0) params[count] = 1.0;
		}
	}
}
