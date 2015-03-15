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
	s.initialState.auxData.resize(s.k);
	copy(s.sequences.begin(), s.sequences.end(), s.initialState.auxData.begin());
	s.buildHeuristicTable();
	//clock_t t1 = clock();
	//SeqNode n = aStarSearch(s);
	//clock_t t2 = clock() - t1;
	//output(n);
	//cout<<t2<<"\n";
	//t1 = clock();
	SeqNode n = dfbb(s);
// 	clock_t t3 = clock() - t1;
 	output(n);
// 	cout<<t3<<"\n";
}

/*
To compile write 
g++ main.cpp
*/