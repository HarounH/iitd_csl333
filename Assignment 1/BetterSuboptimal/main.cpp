#include <iostream>
#include <math.h>
#include <random>
#include <algorithm>
#include <ctime>

using namespace std;

#include "formulation.h"
#include "formulation.cpp"
#include "io.cpp"
#include "hillClimb.cpp"
int main(int argc, char** argv) {
	Problem p;
	input(p); //Get input for p	
	p.init(); //Initialize p.
	State s;
	p.randomState(s , p.initState);
	s = greedyHillClimb_nRestarts_untimed(p , 500);
	p.printState(s);
	return 1;
}