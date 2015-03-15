//#include <math.h> //...because haroun
#include "Sequence Alignment.cpp" //...also includes vectors and also iostream and also the classes of sequence alignment.

#include <queue>
#include <stack> 	//...used for dfs and dfsbb
#include <limits> 	//...used for initialization

#define BIG_DOUBLE std::numeric_limits<double>::max()

struct SeqNodeHeuristicCostComparator { //Used in dfbb to sort the successor nodes.
	bool operator() ( const SeqNode& lhs , const SeqNode& rhs ) { return lhs.heuristicCost > rhs.heuristicCost; }
}seqNodeHeuristicComparatorLeft;

struct SeqNodeTotalCostComparator{
	bool operator() (const SeqNode& lhs , const SeqNode& rhs) { return lhs.totalCost > rhs.totalCost; }
}seqNodeTotalCostComparatorLeft;
//Not passing by reference. Can be changed if needed
SeqNode dfs(SeqProblem& problem)  { //Uninformed search.

	//Initialization
	SeqNode optimalSeqNode; 						// Includes something called auxData which is what we must output.
		optimalSeqNode.totalCost = BIG_DOUBLE; 	//BIG_DOUBLE has been macro-defined as numeric limits.
	//double bound; Not necessary here
	stack<SeqNode> fringe;
		fringe.push(problem.initialState);

	while(!fringe.empty()) {
		//Optimization problem has no "goal state end."
		//Add all the kids to the stack.
		SeqNode node = fringe.top();
		fringe.pop();
		if( problem.goalTest(node) ) {
			if (node.totalCost < optimalSeqNode.totalCost ) {
				optimalSeqNode = node;
			}
		}
		else {
			//Insert all the kids into the stack.
			vector<SeqNode> children;
			problem.getSuccessors(node , children);
			for(int i = 0; i<children.size(); i++) {
				fringe.push(children[i]);
			}
		}
	}
	return optimalSeqNode;
}

SeqNode dfbb(SeqProblem& problem) { //Informed search

	//Initialization.
	//initialize bound.
	SeqNode optimalSeqNode; //Initialized using bound.
		optimalSeqNode.totalCost = BIG_DOUBLE; //BIG_DOUBLE is the numeric limit of double

	// ############# BOUND is the totalCost of the optimalSeqNode;
		int n = 0;
	stack<SeqNode> fringe;
		fringe.push(problem.initialState);

	while( !fringe.empty() ) {
		SeqNode  node = fringe.top();
			fringe.pop();
			//cout<<node.totalCost<<"\n";
		if ( problem.goalTest(node) ) {
			if( node.totalCost < optimalSeqNode.totalCost ) {
				optimalSeqNode = node;
			}
			cout<<node.totalCost<<"\n";
			cout<<n<<"\n";
		}
		else {
			vector<SeqNode> children;
			problem.getSuccessors(node , children);

			//Sort children by their heuristic value. Only heuristic value because looking into the future.
			std::stable_sort(children.begin() , children.end() , seqNodeTotalCostComparatorLeft);
			for(int i = 0; i<children.size(); i++) {
				n++;
				if( children[i].totalCost < optimalSeqNode.totalCost ) {
					fringe.push(children[i]);
				}
			}
		}
	}
	cout<<n<<"\n";
	return optimalSeqNode;
}

SeqNode aStarSearch(SeqProblem& problem) {
	SeqNode optimalSeqNode;
		optimalSeqNode.totalCost = BIG_DOUBLE; //BIG_DOUBLE is the numeric limit of double
		//Follows total cost contour.
	std::priority_queue<SeqNode, vector<SeqNode>, SeqNodeTotalCostComparator> fringe;
		fringe.push(problem.initialState);
		long int n = 0;
		int prev = 0;
	while( !fringe.empty() ) {
		SeqNode node = fringe.top();
			fringe.pop(); //GIVE ME AN EXTRACT FUNCTION DAMMIT.
		if ( problem.goalTest(node) ) {
			if (node.totalCost < optimalSeqNode.totalCost) {
				optimalSeqNode = node;
			}
		}
		else {
			vector<SeqNode> children;
			problem.getSuccessors( node , children );
			for(int i=0; i<children.size(); ++i) {
				n++;
				fringe.push(children[i]);
			}
		}
	}
	return optimalSeqNode;
}

SeqNode dfbbWithGreedyBound( SeqProblem& problem) {
	SeqNode optimalSeqNode;
		optimalSeqNode.heuristicCost = BIG_DOUBLE; 	//BIG_DOUBLE is the numeric limit
	optimalSeqNode.totalCost = BIG_DOUBLE;			//BIG_DOUBLE is the numeric limit

	std::priority_queue<SeqNode, vector<SeqNode>, SeqNodeHeuristicCostComparator> fringe;
		fringe.push(problem.initialState);

	while(!fringe.empty()) {
		SeqNode node = fringe.top();
			fringe.pop(); //Extract() lala
		if ( problem.goalTest(node) ) {
			if( node.totalCost < optimalSeqNode.totalCost ) {
				optimalSeqNode = node;
			}
		}
		else {
			vector<SeqNode> children;
			problem.getSuccessors(node , children);
			for(int i =0 ; i< children.size(); ++i) {
				if( children[i].totalCost < optimalSeqNode.totalCost ) {
					fringe.push(children[i]);
				}
			}
		}
	}
	return optimalSeqNode;
}