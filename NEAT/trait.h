#pragma once
#include <fstream>
#include "neat.h"

namespace NEAT {
	class Trait
	{
	public:
		int trait_id; // Used in file saving and loading
		double params[NEAT::num_trait_params]; // Keep traits in an array

		//Trait();

		//Trait(int id, double p1, double p2, double p3, double p4, double p5, double p6, double p7, double p8, double p9);
		Trait(int id, double p1, double p2, double p3, double p4, double p5, double p6, double p7, double p8, double p9);
		// Copy Constructor
		Trait(const Trait& t);

		// Create a trait exactly like another trait
		Trait(Trait *t);

		// Special constructor off a file assume word "trait" has been read in
		Trait(const char *argline);

		// Special Constructor creates a new Trait which is the average of 2 existing traits passed in
		Trait(Trait *t1, Trait *t2);

		// Dump trait to a stream
		void print_to_file(std::ostream &outFile);
		void print_to_file(std::ofstream &outFile);

		// Perturb the trait parameters slightly
		void mutate();
	};
}


