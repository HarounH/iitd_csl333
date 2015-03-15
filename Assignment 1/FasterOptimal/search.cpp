//#include <math.h> //...because haroun
#include "Sequence Alignment.cpp" //...also includes vectors and also iostream and also the classes of sequence alignment.

#include <queue>
#include <stack> 	//...used for dfs and dfsbb
#include <limits> 	//...used for initialization

#define BIG_DOUBLE std::numeric_limits<double>::max()

struct SeqNodeHeuristicCostComparator { //Used in dfbb to sort the successor nodes.
	bool operator() ( SeqNode *lhs , SeqNode *rhs ) { return lhs->heuristicCost > rhs->heuristicCost; }
}seqNodeHeuristicComparatorLeft;

struct SeqNodeTotalCostComparator{
	bool operator() ( SeqNode *lhs , SeqNode *rhs) { return lhs->totalCost > rhs->totalCost; }
}seqNodeTotalCostComparatorLeft;
//Not passing by reference. Can be changed if needed

SeqNode* dfbb(SeqProblem& problem) { //Informed search

	//Initialization.
	//initialize bound.
	SeqNode *optimalSeqNode; //Initialized using bound.
		double maxCost = BIG_DOUBLE; //BIG_DOUBLE is the numeric limit of double

	// ############# BOUND is the totalCost of the optimalSeqNode;
	stack<SeqNode*> fringe;
	SeqNode *initial = &(problem.initialState);
	fringe.push(initial);
	while( !fringe.empty() ) {
		SeqNode *node = fringe.top();
			fringe.pop();
			//cout<<node.totalCost<<"\n";
			if(node->explored == true)
			{
				if(node->isGoal)
				{
					if(node->parent != NULL)
						node->parent->isGoal = true;
				}
				else
					delete node;
				node = NULL;
				continue;
			}
		if ( problem.goalTest(node) ) {
			if( node->totalCost < maxCost ) {
				node->isGoal = true;
				if(node->parent != NULL)
					node->parent->isGoal = true;
				optimalSeqNode = node;
				maxCost = node->totalCost;
			}
		}
		else {
			if(node->explored == false)
			{
				node->explored = true;
				fringe.push(node);
			}
			vector<SeqNode*> children;
			problem.getSuccessors(node , children);
			//Sort children by their heuristic value. Only heuristic value because looking into the future.
			std::stable_sort(children.begin() , children.end() , seqNodeTotalCostComparatorLeft);
			for(int i = 0; i<children.size(); i++) {
				if( children[i]->totalCost < maxCost ) {
					fringe.push(children[i]);
				}
			}
		}
		node = NULL;
	}
	
	return optimalSeqNode;
}