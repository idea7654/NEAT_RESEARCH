#include "gene.h"

#include <iostream>
#include <sstream>
using namespace NEAT;


NEAT::Gene::Gene(double w, NNode * inode, NNode * onode, bool recur, double innov, double mnum)
{
	lnk = new Link(w, inode, onode, recur);
	innovation_num = innov;
	mutation_num = mnum;

	enable = true;

	frozen = false;
}

NEAT::Gene::Gene(Trait * tp, double w, NNode * inode, NNode * onode, bool recur, double innov, double mnum)
{
	lnk = new Link(tp, w, inode, onode, recur);
	innovation_num = innov;
	mutation_num = mnum;

	enable = true;

	frozen = false;
}

NEAT::Gene::Gene(Gene * g, Trait * tp, NNode * inode, NNode * onode)
{
	lnk = new Link(tp, (g->lnk)->weight, inode, onode, (g->lnk)->is_recurrent);
	innovation_num = g->innovation_num;
	mutation_num = g->mutation_num;
	enable = g->enable;

	frozen = g->frozen;
}

NEAT::Gene::Gene(const char * argline, std::vector<Trait*>& traits, std::vector<NNode*>& nodes)
{
	int traitnum;
	int inodenum;
	int onodenum;
	NNode *inode;
	NNode *onode;
	double weight;
	int recur;
	Trait *traitptr;

	std::vector<Trait*>::iterator curtrait;
	std::vector<NNode*>::iterator curnode;

	//Get the gene parameters

	std::stringstream ss(argline);

	//enable = (bool)(atoi(curword));

	ss >> traitnum >> inodenum >> onodenum >> weight >> recur >> innovation_num >> mutation_num >> enable;

	frozen = false; //TODO: MAYBE CHANGE

	//Get a pointer to the linktrait
	if (traitnum == 0) traitptr = 0;
	else {
		curtrait = traits.begin();
		while (((*curtrait)->trait_id) != traitnum)
			++curtrait;
		traitptr = (*curtrait);
	}

	//Get a pointer to the input node
	curnode = nodes.begin();
	while (((*curnode)->node_id) != inodenum)
		++curnode;
	inode = (*curnode);

	//Get a pointer to the output node
	curnode = nodes.begin();
	while (((*curnode)->node_id) != onodenum)
		++curnode;
	onode = (*curnode);

	lnk = new Link(traitptr, weight, inode, onode, recur);
}

NEAT::Gene::Gene(const Gene & gene)
{
	innovation_num = gene.innovation_num;
	mutation_num = gene.mutation_num;
	enable = gene.enable;
	frozen = gene.frozen;

	lnk = new Link(*gene.lnk);
}

NEAT::Gene::~Gene()
{
	delete lnk;
}

void NEAT::Gene::print_to_file(std::ostream & outFile)
{
	outFile << "gene ";
	//Start off with the trait number for this gene
	if ((lnk->linktrait) == 0) outFile << "0 ";
	else outFile << ((lnk->linktrait)->trait_id) << " ";
	outFile << (lnk->in_node)->node_id << " ";
	outFile << (lnk->out_node)->node_id << " ";
	outFile << (lnk->weight) << " ";
	outFile << (lnk->is_recurrent) << " ";
	outFile << innovation_num << " ";
	outFile << mutation_num << " ";
	outFile << enable << std::endl;
}

void NEAT::Gene::print_to_file(std::ofstream & outFile)
{
	outFile << "gene ";
	//outFile.write(5, "gene ");

	//Start off with the trait number for this gene
	if ((lnk->linktrait) == 0) {
		outFile << "0 ";
		//outFile.write(2, "0 ");
	}
	else {
		outFile << ((lnk->linktrait)->trait_id) << " ";
		//char tempbuf2[128];
		//sprintf(tempbuf2, sizeof(tempbuf2),"%d ", (lnk->linktrait)->trait_id);
		//outFile.write(strlen(tempbuf2),tempbuf2);
	}
	//char tempbuf[1024];
	//sprintf(tempbuf,sizeof(tempbuf),"%d %d %f %d %f %f %d\n", (lnk->in_node)->node_id,
	//	(lnk->out_node)->node_id, lnk->weight, lnk->is_recurrent, innovation_num, mutation_num, enable);
	//outFile.write(strlen(tempbuf),tempbuf);
	outFile << (lnk->in_node)->node_id << " ";
	outFile << (lnk->out_node)->node_id << " ";
	outFile << (lnk->weight) << " ";
	outFile << (lnk->is_recurrent) << " ";
	outFile << innovation_num << " ";
	outFile << mutation_num << " ";
	outFile << enable << std::endl;
}
