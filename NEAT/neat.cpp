#include "neat.h"
#include <cmath>
#include <cstring>

double NEAT::trait_param_mut_prob = 0.5;
double NEAT::trait_mutation_power = 1.0; // Power of mutation on a signle trait param 
double NEAT::linktrait_mut_sig = 1.0; // Amount that mutation_num changes for a trait change inside a link
double NEAT::nodetrait_mut_sig = 0.5; // Amount a mutation_num changes on a link connecting a node that changed its trait 
double NEAT::weight_mut_power = 3; // The power of a linkweight mutation 
double NEAT::recur_prob = 0.2; // Prob. that a link mutation which doesn't have to be recurrent will be made recurrent 
double NEAT::disjoint_coeff = 1.0;
double NEAT::excess_coeff = 1.0;
double NEAT::mutdiff_coeff = 2.0;
double NEAT::compat_threshold = 3.0;
double NEAT::age_significance = 1.0; // How much does age matter? 
double NEAT::survival_thresh = 0.20; // Percent of ave fitness for survival 
double NEAT::mutate_only_prob = 0.25; // Prob. of a non-mating reproduction 
double NEAT::mutate_random_trait_prob = 0.1;
double NEAT::mutate_link_trait_prob = 0.1;
double NEAT::mutate_node_trait_prob = 0.1;
double NEAT::mutate_link_weights_prob = 0.9;
double NEAT::mutate_toggle_enable_prob = 0.00;
double NEAT::mutate_gene_reenable_prob = 0.000;
double NEAT::mutate_add_node_prob = 0.0025;
double NEAT::mutate_add_link_prob = 0.1;
double NEAT::interspecies_mate_rate = 0.05; // Prob. of a mate being outside species 
double NEAT::mate_multipoint_prob = 0.6;
double NEAT::mate_multipoint_avg_prob = 0.4;
double NEAT::mate_singlepoint_prob = 0.0;
double NEAT::mate_only_prob = 0.2; // Prob. of mating without mutation 
double NEAT::recur_only_prob = 0.2;  // Probability of forcing selection of ONLY links that are naturally recurrent 
int NEAT::pop_size = 100;  // Size of population 
int NEAT::dropoff_age = 1000;  // Age where Species starts to be penalized 
int NEAT::newlink_tries = 20;  // Number of tries mutate_add_link will attempt to find an open link 
int NEAT::print_every = 2; // Tells to print population to file every n generations 
int NEAT::babies_stolen = 1; // The number of babies to siphen off to the champions 
int NEAT::num_runs = 1;

int NEAT::getUnitCount(const char * string, const char * set)
{
	int count = 0;
	short last = 0;
	while (*string)
	{
		last = *string++;

		for (int i = 0; set[i]; i++)
		{
			if (last == set[i])
			{
				count++;
				last = 0;
				break;
			}
		}
	}
	if (last)
		count++;
	return count;
}

double NEAT::fsigmoid(double activesum, double slope, double constant)
{
	return (1 / (1 + (exp(-(slope*activesum)))));
}

double NEAT::hebbian(double weight, double maxweight, double active_in, double active_out, double hebb_rate, double pre_rate, double post_rate)
{
	bool neg = false;
	double delta;

	//double weight_mag;

	double topweight;

	if (maxweight < 500000) maxweight = 500000;

	if (weight > maxweight) weight = maxweight;

	if (weight < -maxweight) weight = -maxweight;

	if (weight < 0) {
		neg = true;
		weight = -weight;
	}



	topweight = weight + 200000;
	if (topweight > maxweight) topweight = maxweight;

	if (!(neg)) {
		delta =
			hebb_rate * (maxweight - weight)*active_in*active_out +
			pre_rate * (topweight)*active_in*(active_out - 100000);

		return weight + delta;

	}
	else {
		//In the inhibatory case, we strengthen the synapse when output is low and
		//input is high
		delta =
			pre_rate * (maxweight - weight)*active_in*(100000 - active_out) + //"unhebb"
			//hebb_rate*(maxweight-weight)*(1.0-active_in)*(active_out)+
			-hebb_rate * (topweight + 200000)*active_in*active_out + //anti-hebbian
			//hebb_rate*(maxweight-weight)*active_in*active_out+
			//pre_rate*weight*active_in*(active_out-1.0)+
			//post_rate*weight*(active_in-1.0)*active_out;
			0;


		return -(weight + delta);

	}
}
