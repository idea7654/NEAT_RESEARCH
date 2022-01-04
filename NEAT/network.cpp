#pragma warning(disable : 4996)
#include "network.h"

void NEAT::Network::destroy()
{
	std::vector<NNode*>::iterator curnode;
	std::vector<NNode*>::iterator location;
	std::vector<NNode*> seenlist;  //List of nodes not to doublecount

	// Erase all nodes from all_nodes list 
	for (auto i : all_nodes)
	{
		delete(i);
	}
}

void NEAT::Network::destroy_helper(NNode * curnode, std::vector<NNode*>& seenlist)
{
	std::vector<Link*> innodes = curnode->incoming;
	std::vector<NNode*>::iterator location;

	if (!((curnode->type) == SENSOR)) {
		for (auto i : innodes)
		{
			location = std::find(seenlist.begin(), seenlist.end(), (i->in_node));
			if (location == seenlist.end()) {
				seenlist.push_back(i->in_node);
				destroy_helper(i->in_node, seenlist);
			}
		}
	}
}

void NEAT::Network::nodecounthelper(NNode * curnode, int & counter, std::vector<NNode*>& seenlist)
{
	std::vector<Link*> innodes = curnode->incoming;
	std::vector<NNode*>::iterator location;

	if (!((curnode->type) == SENSOR)) {
		for (auto &i : innodes)
		{
			location = std::find(seenlist.begin(), seenlist.end(), i->in_node);
			if (location == seenlist.end()) {
				counter++;
				seenlist.push_back(i->in_node);
				nodecounthelper(i->in_node, counter, seenlist);
			}
		}
	}
}

void NEAT::Network::linkcounthelper(NNode * curnode, int & counter, std::vector<NNode*>& seenlist)
{
	std::vector<Link*> inlinks = curnode->incoming;
	std::vector<NNode*>::iterator location;

	location = std::find(seenlist.begin(), seenlist.end(), curnode);
	if ((!((curnode->type) == SENSOR)) && (location == seenlist.end())) {
		seenlist.push_back(curnode);

		for (auto i : inlinks) {
			counter++;
			linkcounthelper(i->in_node, counter, seenlist);
		}

	}
}

NEAT::Network::Network(std::vector<NNode*> in, std::vector<NNode*> out, std::vector<NNode*> all, int netid)
{
	inputs = in;
	outputs = out;
	all_nodes = all;
	name = 0;   //Defaults to no name  ..NOTE: TRYING TO PRINT AN EMPTY NAME CAN CAUSE A CRASH
	numnodes = -1;
	numlinks = -1;
	net_id = netid;
	adaptable = false;
}

NEAT::Network::Network(std::vector<NNode*> in, std::vector<NNode*> out, std::vector<NNode*> all, int netid, bool adaptval)
{
	inputs = in;
	outputs = out;
	all_nodes = all;
	name = 0;   //Defaults to no name  ..NOTE: TRYING TO PRINT AN EMPTY NAME CAN CAUSE A CRASH                                    
	numnodes = -1;
	numlinks = -1;
	net_id = netid;
	adaptable = adaptval;
}

NEAT::Network::Network(int netid)
{
	name = 0; //Defaults to no name
	numnodes = -1;
	numlinks = -1;
	net_id = netid;
	adaptable = false;
}

NEAT::Network::Network(int netid, bool adaptval)
{
	name = 0; //Defaults to no name                                                                                               
	numnodes = -1;
	numlinks = -1;
	net_id = netid;
	adaptable = adaptval;
}

NEAT::Network::Network(const Network & network)
{
	//std::vector<NNode*>::const_iterator curnode;

	// Copy all the inputs
	/*for (curnode = network.inputs.begin(); curnode != network.inputs.end(); ++curnode) {
		NNode* n = new NNode(**curnode);
		inputs.push_back(n);
		all_nodes.push_back(n);
	}*/

	for (auto i : network.inputs)
	{
		NNode *n = new NNode(*i);
		inputs.push_back(n);
		all_nodes.push_back(n);
	}

	// Copy all the outputs
	/*for (curnode = network.outputs.begin(); curnode != network.outputs.end(); ++curnode) {
		NNode* n = new NNode(**curnode);
		outputs.push_back(n);
		all_nodes.push_back(n);
	}*/
	for (auto i : network.outputs)
	{
		NNode *n = new NNode(*i);
		outputs.push_back(n);
		all_nodes.push_back(n);
	}

	if (network.name)
		name = strdup(network.name);
	else
		name = 0;

	numnodes = network.numnodes;
	numlinks = network.numlinks;
	net_id = network.net_id;
	adaptable = network.adaptable;
}

NEAT::Network::~Network()
{
	if (name != 0)
		delete[] name;

	destroy();  // Kill off all the nodes and links
}

void NEAT::Network::flush()
{
	std::vector<NNode*>::iterator curnode;

	//for (curnode = outputs.begin(); curnode != outputs.end(); ++curnode) {
	//	(*curnode)->flushback();
	//}
	for (auto &i : outputs)
	{
		i->flushback();
	}
}

void NEAT::Network::flush_check()
{
	//std::vector<NNode*>::iterator curnode;
	std::vector<NNode*>::iterator location;
	std::vector<NNode*> seenlist;  //List of nodes not to doublecount

	/*for (curnode = outputs.begin(); curnode != outputs.end(); ++curnode) {
		location = std::find(seenlist.begin(), seenlist.end(), (*curnode));
		if (location == seenlist.end()) {
			seenlist.push_back(*curnode);
			(*curnode)->flushback_check(seenlist);
		}
	}*/

	for (auto i : outputs)
	{
		location = std::find(seenlist.begin(), seenlist.end(), i);
		if (location == seenlist.end())
		{
			seenlist.push_back(i);
			i->flushback_check(seenlist);
		}
	}
}

bool NEAT::Network::activate()
{
	std::vector<NNode*>::iterator curnode;
	std::vector<Link*>::iterator curlink;
	double add_amount;  //For adding to the activesum
	bool onetime; //Make sure we at least activate once
	int abortcount = 0;  //Used in case the output is somehow truncated from the network

	//cout<<"Activating network: "<<this->genotype<<endl;

	//Keep activating until all the outputs have become active 
	//(This only happens on the first activation, because after that they
	// are always active)

	onetime = false;

	while (outputsoff() || !onetime) {

		++abortcount;

		if (abortcount == 20) {
			return false;
			//cout<<"Inputs disconnected from output!"<<endl;
		}
		//std::cout<<"Outputs are off"<<std::endl;

		// For each node, compute the sum of its incoming activation 
		//for (curnode = all_nodes.begin(); curnode != all_nodes.end(); ++curnode) {
		//	//Ignore SENSORS

		//	//cout<<"On node "<<(*curnode)->node_id<<endl;

		//	if (((*curnode)->type) != SENSOR) {
		//		(*curnode)->activesum = 0;
		//		(*curnode)->active_flag = false;  //This will tell us if it has any active inputs

		//		// For each incoming connection, add the activity from the connection to the activesum 
		//		for (curlink = ((*curnode)->incoming).begin(); curlink != ((*curnode)->incoming).end(); ++curlink) {
		//			//Handle possible time delays
		//			if (!((*curlink)->time_delay)) {
		//				add_amount = ((*curlink)->weight)*(((*curlink)->in_node)->get_active_out());
		//				if ((((*curlink)->in_node)->active_flag) ||
		//					(((*curlink)->in_node)->type == SENSOR)) (*curnode)->active_flag = true;
		//				(*curnode)->activesum += add_amount;
		//				//std::cout<<"Node "<<(*curnode)->node_id<<" adding "<<add_amount<<" from node "<<((*curlink)->in_node)->node_id<<std::endl;
		//			}
		//			else {
		//				//Input over a time delayed connection
		//				add_amount = ((*curlink)->weight)*(((*curlink)->in_node)->get_active_out_td());
		//				(*curnode)->activesum += add_amount;
		//			}

		//		} //End for over incoming links

		//	} //End if (((*curnode)->type)!=SENSOR) 

		//} //End for over all nodes

		for (auto &i : all_nodes)
		{
			if (i->type != SENSOR)
			{
				i->activesum = 0;
				i->active_flag = false;

				for (auto &j : i->incoming)
				{
					if (!j->time_delay)
					{
						add_amount = j->weight * j->in_node->get_active_out();
						if ((j->in_node)->active_flag || (j->in_node)->type == SENSOR)
							i->active_flag = true;
						i->activesum += add_amount;
					}
					else
					{
						add_amount = j->weight * (j->in_node)->get_active_out_td();
						i->activesum += add_amount;
					}
				}
			}
		}

		// Now activate all the non-sensor nodes off their incoming activation 
		//for (curnode = all_nodes.begin(); curnode != all_nodes.end(); ++curnode) {

		//	if (((*curnode)->type) != SENSOR) {
		//		//Only activate if some active input came in
		//		if ((*curnode)->active_flag) {
		//			//cout<<"Activating "<<(*curnode)->node_id<<" with "<<(*curnode)->activesum<<": ";

		//			//Keep a memory of activations for potential time delayed connections
		//			(*curnode)->last_activation2 = (*curnode)->last_activation;
		//			(*curnode)->last_activation = (*curnode)->activation;

		//			//If the node is being overrided from outside,
		//			//stick in the override value
		//			if ((*curnode)->overridden()) {
		//				//Set activation to the override value and turn off override
		//				(*curnode)->activate_override();
		//			}
		//			else {
		//				//Now run the net activation through an activation function
		//				if ((*curnode)->ftype == SIGMOID)
		//					(*curnode)->activation = NEAT::fsigmoid((*curnode)->activesum, 4.924273, 2.4621365);  //Sigmoidal activation- see comments under fsigmoid
		//			}
		//			//cout<<(*curnode)->activation<<endl;

		//			//Increment the activation_count
		//			//First activation cannot be from nothing!!
		//			(*curnode)->activation_count++;
		//		}
		//	}
		//}

		for (auto &i : all_nodes)
		{
			if (i->type != SENSOR)
			{
				if (i->active_flag)
				{
					i->last_activation2 = i->last_activation;
					i->last_activation = i->activation;

					if (i->overridden())
					{
						i->activate_override();
					}
					else
					{
						if (i->ftype == SIGMOID)
							i->activation = fsigmoid(i->activesum, 4.924273, 2.4621365);
					}
					i->activation_count++;
				}
			}
		}

		onetime = true;
	}

	if (adaptable) {

		//std::cout << "ADAPTING" << std:endl;

		// ADAPTATION:  Adapt weights based on activations 
		for (auto &i : all_nodes)
		{
			if (i->type != SENSOR)
			{
				for (auto j : i->incoming)
				{
					if (j->trait_id == 2 || j->trait_id == 3 || j->trait_id == 4)
					{
						if (j->is_recurrent)
						{
							j->weight = hebbian(j->weight, maxweight, j->in_node->last_activation, j->out_node->get_active_out(), j->params[0], j->params[1], j->params[2]);
						}
						else
						{
							j->weight = hebbian(j->weight, maxweight, j->in_node->get_active_out(), j->out_node->get_active_out(), j->params[0], j->params[1], j->params[2]);
						}
					}
				}
			}
		}

	} //end if (adaptable)

	return true;
}

void NEAT::Network::show_activation()
{
	int count;

	count = 1;

	for (auto &i : outputs)
		count++;
}

void NEAT::Network::show_input()
{
}

void NEAT::Network::add_input(NNode *in_node)
{
	inputs.push_back(in_node);
}

void NEAT::Network::add_output(NNode *out_node)
{
	outputs.push_back(out_node);
}

void NEAT::Network::load_sensors(double *sensvals)
{
	for (auto &i : inputs)
	{
		if (i->type == SENSOR)
		{
			i->sensor_load(*sensvals);
			sensvals++;
		}
	}
}

void NEAT::Network::load_sensors(const std::vector<float>& sensvals)
{
	std::vector<NNode*>::iterator sensPtr;
	std::vector<float>::const_iterator valPtr;

	for (valPtr = sensvals.begin(), sensPtr = inputs.begin(); sensPtr != inputs.end() && valPtr != sensvals.end(); ++sensPtr, ++valPtr) {
		//only load values into SENSORS (not BIASes)
		if (((*sensPtr)->type) == SENSOR) {
			(*sensPtr)->sensor_load(*valPtr);
			//sensvals++;
		}
	}
}

void NEAT::Network::override_outputs(double *outvals)
{
	for (auto &i : outputs)
	{
		i->override_output(*outvals);
		outvals++;
	}
}

void NEAT::Network::give_name(char *newname)
{
	char *temp;
	char *temp2;
	temp = new char[strlen(newname) + 1];
	strcpy(temp, newname);
	if (name == 0) name = temp;
	else {
		temp2 = name;
		delete temp2;
		name = temp;
	}
}

int NEAT::Network::nodecount()
{
	int counter = 0;
	std::vector<NNode*>::iterator location;
	std::vector<NNode*> seenlist;  //List of nodes not to doublecount

	for (auto &i : outputs) {

		location = std::find(seenlist.begin(), seenlist.end(), i);
		if (location == seenlist.end()) {
			counter++;
			seenlist.push_back(i);
			nodecounthelper(i, counter, seenlist);
		}
	}

	numnodes = counter;

	return counter;
}

int NEAT::Network::linkcount()
{
	int counter = 0;
	std::vector<NNode*> seenlist;  //List of nodes not to doublecount

	for (auto i : outputs)
	{
		linkcounthelper(i, counter, seenlist);
	}

	numlinks = counter;

	return counter;
}

bool NEAT::Network::is_recur(NNode * potin_node, NNode * potout_node, int & count, int thresh)
{
	++count;  //Count the node as visited

	if (count > thresh) {
		//cout<<"returning false"<<endl;
		return false;  //Short out the whole thing- loop detected
	}

	if (potin_node == potout_node) return true;
	else {
		//Check back on all links...
		for (auto &i : potin_node->incoming) {
			//But skip links that are already recurrent
			//(We want to check back through the forward flow of signals only
			if (!(i->is_recurrent)) {
				if (is_recur(i->in_node, potout_node, count, thresh)) return true;
			}
		}
		return false;
	}
}

int NEAT::Network::input_start()
{
	input_iter = inputs.begin();
	return 1;
}

int NEAT::Network::load_in(double d)
{
	(*input_iter)->sensor_load(d);
	input_iter++;
	if (input_iter == inputs.end()) return 0;
	else return 1;
}

bool NEAT::Network::outputsoff()
{
	//std::vector<NNode*>::iterator curnode;

	//for (curnode = outputs.begin(); curnode != outputs.end(); ++curnode) {
	//	if (((*curnode)->activation_count) == 0) return true;
	//}
	for (auto &i : outputs)
	{
		if (i->activation_count == 0)
			return true;
	}

	return false;
}

void NEAT::Network::print_links_tofile(char * filename)
{
	std::vector<NNode*>::iterator curnode;
	std::vector<Link*>::iterator curlink;

	std::ofstream oFile(filename);

	for (curnode = all_nodes.begin(); curnode != all_nodes.end(); ++curnode) {
		if (((*curnode)->type) != SENSOR) {
			for (curlink = ((*curnode)->incoming).begin(); curlink != ((*curnode)->incoming).end(); ++curlink) {
				oFile << (*curlink)->in_node->node_id << " -> " << (*curlink)->out_node->node_id << " : " << (*curlink)->weight << std::endl;
			} // end for loop on links
		} //end if
	} //end for loop on nodes

	for (auto i : all_nodes)
	{
		if (i->type != SENSOR)
		{
			for (auto j : i->incoming)
			{
				oFile << j->in_node->node_id << " -> " << j->out_node->node_id << " : " << j->weight << std::endl;
			}
		}
	}

	oFile.close();
}

int NEAT::Network::max_depth()
{
	int cur_depth; //The depth of the current node
	int max = 0; //The max depth

	for (auto i : outputs) {
		cur_depth = i->depth(0, this);
		if (cur_depth > max) max = cur_depth;
	}

	return max;
}
