#pragma once

#include <cstdlib>
#include <cstring>

namespace NEAT {
	const int num_trait_params = 8;

	extern double trait_param_mut_prob;
	extern double trait_mutation_power; // Power of mutation on a signle trait param 
	extern double linktrait_mut_sig;  // Amount that mutation_num changes for a trait change inside a link
	extern double nodetrait_mut_sig; // Amount a mutation_num changes on a link connecting a node that changed its trait 
	extern double weight_mut_power;  // The power of a linkweight mutation 
	extern double recur_prob;        // Prob. that a link mutation which doesn't have to be recurrent will be made recurrent 

	// These 3 global coefficients are used to determine the formula for
	// computating the compatibility between 2 genomes.  The formula is:
	// disjoint_coeff*pdg+excess_coeff*peg+mutdiff_coeff*mdmg.
	// See the compatibility method in the Genome class for more info
	// They can be thought of as the importance of disjoint Genes,
	// excess Genes, and parametric difference between Genes of the
	// same function, respectively. 
	extern double disjoint_coeff;
	extern double excess_coeff;
	extern double mutdiff_coeff;

	// This global tells compatibility threshold under which two Genomes are considered the same species 
	extern double compat_threshold;

	// Globals involved in the epoch cycle - mating, reproduction, etc.. 
	extern double age_significance;          // How much does age matter? 
	extern double survival_thresh;           // Percent of ave fitness for survival 
	extern double mutate_only_prob;          // Prob. of a non-mating reproduction 
	extern double mutate_random_trait_prob;
	extern double mutate_link_trait_prob;
	extern double mutate_node_trait_prob;
	extern double mutate_link_weights_prob;
	extern double mutate_toggle_enable_prob;
	extern double mutate_gene_reenable_prob;
	extern double mutate_add_node_prob;
	extern double mutate_add_link_prob;
	extern double interspecies_mate_rate;    // Prob. of a mate being outside species 
	extern double mate_multipoint_prob;
	extern double mate_multipoint_avg_prob;
	extern double mate_singlepoint_prob;
	extern double mate_only_prob;            // Prob. of mating without mutation 
	extern double recur_only_prob;  // Probability of forcing selection of ONLY links that are naturally recurrent 
	extern int pop_size;  // Size of population 
	extern int dropoff_age;  // Age where Species starts to be penalized 
	extern int newlink_tries;  // Number of tries mutate_add_link will attempt to find an open link 
	extern int print_every; // Tells to print population to file every n generations 
	extern int babies_stolen; // The number of babies to siphon off to the champions 

	extern int num_runs; //number of times to run experiment

	//const char *getUnit(const char *string, int index, const char *set);
	//const char *getUnits(const char *string, int startIndex, int endIndex, const char *set);
	int getUnitCount(const char *string, const char *set);

	double fsigmoid(double activesum, double slope, double constant);
	double hebbian(double weight, double maxweight, double active_in, double active_out, double hebb_rate, double pre_rate, double post_rate);
}

