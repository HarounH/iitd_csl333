#include <ctime>		//...Allows us to time stuff.
#include <random>
#include "InputOutput.cpp"
#include <algorithm>
#include "hillClimb.cpp"

int main(int argc, char** argv) {
	clock_t start;
	start = clock();
	cout << CLOCKS_PER_SEC;
	SeqProblem s;
	input(s);
	s.initialize(RANDOM);
	vector<SeqState> nbd;
	//SeqState ans = greedyHillClimb_infRestarts_timed(s , start , STATS);
	//s.printState(  ans );
	return 1;
}