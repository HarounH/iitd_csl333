#include "Sequence Alignment.cpp"
#include "InputOutput.cpp"
#include <algorithm>
#include "search.cpp"
#include <ctime>

int main(int argc, char** argv) {
	SeqProblem s;
	input(s);
	s.initialState.position.resize(s.k);
	s.initialState.currentCost = 0;
	s.initialState.heuristicCost = 0;
	s.initialState.totalCost = 0;
	s.initialState.parent = NULL;
	s.initialState.explored = false;
	s.initialState.isGoal = true;
	s.buildHeuristicTable();
	//clock_t t1 = clock();
	//SeqNode n = aStarSearch(s);
	//clock_t t2 = clock() - t1;
	//output(n);
	//cout<<t2<<"\n";
	//t1 = clock();
	SeqNode *n = dfbb(s);
// 	clock_t t3 = clock() - t1;
 	output(n, s);
// 	cout<<t3<<"\n";
}

/*
To compile write 
g++ main.cpp
*/